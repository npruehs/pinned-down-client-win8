#include "pch.h"

#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>

#include "PinnedDownResourceManager.h"

#include "Events\ResourceLoadedEvent.h"

#include "Resources\BitmapResourceHandle.h"
#include "Resources\AudioResourceHandle.h"
#include "Resources\TextResourceHandle.h"

#include "Util\DirectXUtils.h"
#include "Util\MemoryUtils.h"
#include "Util\StringUtils.h"


using namespace concurrency;

using namespace Windows::ApplicationModel;
using namespace Windows::Storage;

using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Resources;
using namespace PinnedDownClient::Util;


PinnedDownResourceManager::PinnedDownResourceManager(Game* game) : ResourceManager(game)
{
	// Create WIC imaging factory.
	ThrowIfFailed(
		CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&this->wicImagingFactory
		)
		);
}

void PinnedDownResourceManager::LoadBitmapFromFile(
	ID2D1DeviceContext* d2dContext,
	PCWSTR imageUri
	)
{
	// Create an IWICBitmapDecoder.
	IWICBitmapDecoder *decoder = NULL;
	ThrowIfFailed(
		this->wicImagingFactory->CreateDecoderFromFilename(
		imageUri,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&decoder
		)
		);

	// Create the initial frame.
	IWICBitmapFrameDecode *source = NULL;
	ThrowIfFailed(
		decoder->GetFrame(0, &source)
		);

	// Convert the image format to 32bppPBGRA
	// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
	IWICFormatConverter *converter = NULL;
	ThrowIfFailed(
		this->wicImagingFactory->CreateFormatConverter(&converter)
		);

	ThrowIfFailed(
		converter->Initialize(
		source,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.f,
		WICBitmapPaletteTypeMedianCut
		)
		);

	// Create a Direct2D bitmap from the WIC bitmap.
	ID2D1Bitmap *bitmap;
	ThrowIfFailed(
		d2dContext->CreateBitmapFromWicBitmap(
		converter,
		NULL,
		&bitmap
		)
		);

	// Add to resource map.
	ResHandlePtr handle = std::make_shared<BitmapResourceHandle>(WStringToString(imageUri), bitmap);
	this->resourceMap.insert(std::pair<unsigned long, ResHandlePtr>(handle->GetResourceName()->GetHash(), handle));

	// Release resources.
	SafeRelease(&decoder);
	SafeRelease(&source);
	SafeRelease(&converter);
}

void PinnedDownResourceManager::LoadAudioFromFile(IXAudio2* engine, LPCWSTR audioUri)
{
	// Create a Media Foundation object and process the media file.
	Microsoft::WRL::ComPtr<IMFSourceReader> reader;

	// Initialize Microsoft Media Foundation.
	ThrowIfFailed(MFStartup(MF_VERSION));

	// Create the source reader on the specified url. This will file be loaded entirely into memory,
	// so the low latency attribute is not required here; if you are attempting to stream
	// sound effects from disk, the low latency attribute should be set.
	ThrowIfFailed(MFCreateSourceReaderFromURL(audioUri, nullptr, &reader));

	// Set the decoded output format as PCM.
	// XAudio2 on Windows can process PCM and ADPCM-encoded buffers.
	// When using MF, this sample always decodes into PCM.
	// Note the inbox ADPCM decoder supports sample rates only up to 44.1KHz. 
	Microsoft::WRL::ComPtr<IMFMediaType> mediaType;
	ThrowIfFailed(MFCreateMediaType(&mediaType));
	ThrowIfFailed(mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio));
	ThrowIfFailed(mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM));
	ThrowIfFailed(reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, mediaType.Get()));

	// Get the complete WAVEFORMAT from the Media Type.
	Microsoft::WRL::ComPtr<IMFMediaType> outputMediaType;
	ThrowIfFailed(reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, outputMediaType.GetAddressOf()));

	uint32 formatByteCount = 0;
	WAVEFORMATEX* waveFormat;
	ThrowIfFailed(MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &formatByteCount));

	// Buffer the sound data in-memory.
	std::shared_ptr<std::vector<BYTE>> resultData = std::make_shared<std::vector<BYTE>>();

	for (;;)
	{
		DWORD flags = 0;
		Microsoft::WRL::ComPtr<IMFSample> sample;
		ThrowIfFailed(
			reader->ReadSample(
			MF_SOURCE_READER_FIRST_AUDIO_STREAM,
			0,
			nullptr,
			&flags,
			nullptr,
			&sample
			)
			);

		if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
		{
			// End of stream.
			break;
		}

		Microsoft::WRL::ComPtr<IMFMediaBuffer> mediaBuffer;
		ThrowIfFailed(
			sample->ConvertToContiguousBuffer(&mediaBuffer)
			);

		// Serialize access to the buffer by calling the IMFMediaBuffer::Lock method.
		uint8* audioData = nullptr;
		DWORD sampleBufferLength = 0;
		ThrowIfFailed(mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength));

		// Append the new buffer to the running total.
		size_t lastBufferSize = resultData->size();
		resultData->resize(lastBufferSize + sampleBufferLength);
		std::copy(audioData, audioData + sampleBufferLength, resultData->begin() + lastBufferSize);

		// Release the lock on the buffer.
		ThrowIfFailed(mediaBuffer->Unlock());
	}

	if (resultData->size() > XAUDIO2_MAX_BUFFER_BYTES)
	{
		ThrowIfFailed(E_FAIL);
	}

	// Add to resource map.
	ResHandlePtr handle = std::make_shared<AudioResourceHandle>(WStringToString(audioUri), waveFormat, resultData);
	this->resourceMap.insert(std::pair<unsigned long, ResHandlePtr>(handle->GetResourceName()->GetHash(), handle));
}

void PinnedDownResourceManager::LoadTextFromFile(std::string fileUri)
{
	StorageFolder^ storageFolder = Windows::ApplicationModel::Package::Current->InstalledLocation;
	auto getFolderTask = create_task(storageFolder->GetFolderAsync("Assets"));

	getFolderTask.then([this, fileUri](StorageFolder^ folder)
	{
		return folder->GetFolderAsync("Text");
	}).then([this, fileUri](StorageFolder^ folder)
	{
		auto fileUriW = StringToWString(fileUri);
		return folder->GetFileAsync(ref new Platform::String(fileUriW.c_str()));
	}).then([this, fileUri](StorageFile^ textFile)
	{
		return FileIO::ReadTextAsync(textFile);
	}).then([this, fileUri](task<Platform::String^> task)
	{
		Platform::String^ fileContent = task.get();
		std::string text = WStringToString(fileContent->Data());

		// Add to resource map.
		ResHandlePtr handle = std::make_shared<TextResourceHandle>(fileUri, text);
		this->resourceMap.insert(std::pair<unsigned long, ResHandlePtr>(handle->GetResourceName()->GetHash(), handle));

		// Notify listeners.
		auto resourceLoadedEvent = std::make_shared<ResourceLoadedEvent>(fileUri);
		this->game->eventManager->QueueEvent(resourceLoadedEvent);
	});
}

Platform::Array<byte>^ PinnedDownResourceManager::ReadBytes(Platform::String^ fileName)
{
	// http://msdn.microsoft.com/de-de/library/windows/apps/jj651549.aspx
	Windows::Storage::StorageFolder^ m_location = Package::Current->InstalledLocation;
	Platform::String^ m_locationPath = Platform::String::Concat(m_location->Path, "\\");

	CREATEFILE2_EXTENDED_PARAMETERS extendedParams = { 0 };
	extendedParams.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
	extendedParams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
	extendedParams.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
	extendedParams.dwSecurityQosFlags = SECURITY_ANONYMOUS;
	extendedParams.lpSecurityAttributes = nullptr;
	extendedParams.hTemplateFile = nullptr;

	Wrappers::FileHandle file(
		CreateFile2(
		Platform::String::Concat(m_locationPath, fileName)->Data(),
		GENERIC_READ,
		FILE_SHARE_READ,
		OPEN_EXISTING,
		&extendedParams
		)
		);
	if (file.Get() == INVALID_HANDLE_VALUE)
	{
		throw ref new Platform::FailureException();
	}

	FILE_STANDARD_INFO fileInfo = { 0 };
	if (!GetFileInformationByHandleEx(
		file.Get(),
		FileStandardInfo,
		&fileInfo,
		sizeof(fileInfo)
		))
	{
		throw ref new Platform::FailureException();
	}

	if (fileInfo.EndOfFile.HighPart != 0)
	{
		throw ref new Platform::OutOfMemoryException();
	}

	Platform::Array<byte>^ fileData = ref new Platform::Array<byte>(fileInfo.EndOfFile.LowPart);

	if (!ReadFile(
		file.Get(),
		fileData->Data,
		fileData->Length,
		nullptr,
		nullptr
		))
	{
		throw ref new Platform::FailureException();
	}

	return fileData;
}