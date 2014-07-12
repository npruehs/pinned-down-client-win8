#include "pch.h"
#include "Core\ResourceManager.h"
#include "Helpers\DirectXHelper.h"
#include "Util\MemoryUtils.h"

using namespace PinnedDownClient::Core;
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

void ResourceManager::LoadBitmapFromFile(
	ID2D1DeviceContext* d2dContext,
	PCWSTR imageUri,
	UINT destinationWidth,
	UINT destinationHeight,
	ID2D1Bitmap **bitmap
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
	DX::ThrowIfFailed(
		d2dContext->CreateBitmapFromWicBitmap(
		converter,
		NULL,
		bitmap
		)
		);

	SafeRelease(&decoder);
	SafeRelease(&source);
	SafeRelease(&converter);
}