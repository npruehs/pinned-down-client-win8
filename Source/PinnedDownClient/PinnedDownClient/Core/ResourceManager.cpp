#include "pch.h"
#include "Core\ResourceManager.h"
#include "Helpers\DirectXHelper.h"
#include "Util\MemoryUtils.h"
#include "Core\GameException.h"
#include "Core\Resources\BitmapResourceHandle.h"

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