#include "pch.h"
#include "Core\ResourceManager.h"
#include "Helpers\DirectXHelper.h"
#include "Util\MemoryUtils.h"
#include "Core\GameException.h"
#include "Core\Resources\BitmapResourceHandle.h"

using namespace Windows::ApplicationModel;

using namespace PinnedDownClient::Core;
using namespace PinnedDownClient::Core::Resources;
using namespace PinnedDownClient::Util;

ResourceManager::ResourceManager()
{
	// Create WIC imaging factory.
	DX::ThrowIfFailed(
		CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&this->wicImagingFactory
		)
		);
}

ResHandlePtr ResourceManager::GetResource(HashedString resourceName)
{
	// Lookup resource.
	std::map<unsigned long, ResHandlePtr>::iterator iterator = this->resourceMap.find(resourceName.getHash());

	if (iterator != this->resourceMap.end())
	{
		// Return handle.
		return ResHandlePtr(iterator->second);
	}
	else
	{
		// Resource not found.
		std::string errorMsg = "Resource not found: ";
		errorMsg.append(resourceName.getString());
		throw GameException(errorMsg);
	}
}

void ResourceManager::LoadBitmapFromFile(
	ID2D1DeviceContext* d2dContext,
	const char* resourceUri,
	PCWSTR imageUri
	)
{
	// Create an IWICBitmapDecoder.
	IWICBitmapDecoder *decoder = NULL;
	DX::ThrowIfFailed(
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
	DX::ThrowIfFailed(
		decoder->GetFrame(0, &source)
		);

	// Convert the image format to 32bppPBGRA
	// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
	IWICFormatConverter *converter = NULL;
	DX::ThrowIfFailed(
		this->wicImagingFactory->CreateFormatConverter(&converter)
		);

	DX::ThrowIfFailed(
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
	DX::ThrowIfFailed(
		d2dContext->CreateBitmapFromWicBitmap(
		converter,
		NULL,
		&bitmap
		)
		);

	// Add to resource map.
	HashedString hashedFileName = HashedString(resourceUri);
	unsigned long hash = hashedFileName.getHash();

	ResHandlePtr handle = ResHandlePtr(new BitmapResourceHandle(resourceUri, bitmap));
	this->resourceMap.insert(std::pair<unsigned long, ResHandlePtr>(hash, handle));

	// Release resources.
	SafeRelease(&decoder);
	SafeRelease(&source);
	SafeRelease(&converter);
}

Platform::Array<byte>^ ResourceManager::ReadBytes(Platform::String^ fileName)
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