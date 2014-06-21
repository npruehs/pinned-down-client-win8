
#include "pch.h"
#include "Core\Event.h"
#include "Core\EventManager.h"
#include "Systems\RenderSystem.h"

using namespace PinnedDownClient::Systems;

RenderSystem::RenderSystem()
{
}

void RenderSystem::InitSystem(std::shared_ptr<EventManager> eventManager)
{
	ISystem::InitSystem(eventManager);

	eventManager->AddListener(std::shared_ptr<IEventListener>(this), Util::HashedString("AppWindowChanged"));
	eventManager->AddListener(std::shared_ptr<IEventListener>(this), Util::HashedString("AppSuspending"));
	eventManager->AddListener(std::shared_ptr<IEventListener>(this), Util::HashedString("AppWindowSizeChanged"));
}

void RenderSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == Util::HashedString("AppWindowChanged"))
	{
		Events::AppWindowChangedEvent appWindowChangedEvent = static_cast<Events::AppWindowChangedEvent&>(newEvent);
		this->OnAppWindowChanged(appWindowChangedEvent);
	}
	else if (newEvent.GetEventType() == Util::HashedString("AppSuspending"))
	{
		this->OnAppSuspending();
	}
	else if (newEvent.GetEventType() == Util::HashedString("AppWindowSizeChanged"))
	{
		Events::AppWindowSizeChangedEvent appWindowSizeChangedEvent = static_cast<Events::AppWindowSizeChangedEvent&>(newEvent);
		this->OnAppWindowSizeChanged(appWindowSizeChangedEvent);
	}
}

void RenderSystem::OnAppWindowChanged(Events::AppWindowChangedEvent appWindowChangedEvent)
{
	this->window = appWindowChangedEvent.appWindow;

	// Create devices.
	this->CreateD3DDevice();
	this->CreateD2DDevice();

	// Set render target.
	this->CreateSwapChain();
	this->SetRenderTarget();
}

void RenderSystem::OnAppSuspending()
{
	// Provides a hint to the driver that the app is entering an idle state and that temporary buffers can be reclaimed for use by other apps.
	ComPtr<IDXGIDevice3> dxgiDevice;
	this->d3dDevice.As(&dxgiDevice);

	dxgiDevice->Trim();
}

void RenderSystem::OnAppWindowSizeChanged(Events::AppWindowSizeChangedEvent appWindowSizeChangedEvent)
{
	// Clear the previous window size specific context.
	this->d2dContext->SetTarget(nullptr);
	this->d2dTargetBitmap = nullptr;
	this->d3dContext->Flush();

	// Calculate the necessary render target size in pixels (for CoreWindow).
	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	float currentWidth = DX::ConvertDipsToPixels(appWindowSizeChangedEvent.width, currentDisplayInformation->LogicalDpi);
	float currentHeight = DX::ConvertDipsToPixels(appWindowSizeChangedEvent.height, currentDisplayInformation->LogicalDpi);

	// Prevent zero size DirectX content from being created.
	currentWidth = max(currentWidth, 1);
	currentHeight = max(currentHeight, 1);

	// The width and height of the swap chain must be based on the window's
	// natively-oriented width and height. If the window is not in the native
	// orientation, the dimensions must be reversed.
	DXGI_MODE_ROTATION displayRotation = ComputeDisplayRotation(currentDisplayInformation->NativeOrientation, currentDisplayInformation->CurrentOrientation);

	bool swapDimensions = displayRotation == DXGI_MODE_ROTATION_ROTATE90 || displayRotation == DXGI_MODE_ROTATION_ROTATE270;
	float newWidth = swapDimensions ? currentHeight : currentWidth;
	float newHeight = swapDimensions ? currentWidth : currentHeight;

	if (this->dxgiSwapChain != nullptr)
	{
		// If the swap chain already exists, resize it.
		HRESULT hr = this->dxgiSwapChain->ResizeBuffers(
			this->swapChainBufferCount,
			static_cast<UINT>(newWidth),
			static_cast<UINT>(newHeight),
			DXGI_FORMAT_B8G8R8A8_UNORM,
			0
			);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			// If the device was removed for any reason, a new device and swap chain will need to be created.
			this->OnDeviceLost();

			// Everything is set up now. Do not continue execution of this method.
			// HandleDeviceLost will reenter this method and correctly set up the new device.
			return;
		}
		else
		{
			DX::ThrowIfFailed(hr);
		}
	}
	else
	{
		// Otherwise, create a new one using the same adapter as the existing Direct3D device.
		this->CreateSwapChain();
	}

	// Set the proper orientation for the swap chain.
	// TODO(np): Always set swap chain rotation?
	DX::ThrowIfFailed(
		this->dxgiSwapChain->SetRotation(displayRotation)
		);

	// Create a Direct2D target bitmap associated with the
	// swap chain back buffer and set it as the current target.
	this->SetRenderTarget();
}

void RenderSystem::CreateD3DDevice()
{
	// Direct2D requires BGRA color order.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	// Set of DirectX hardware feature levels this app supports. Preserve order.
	// App's minimum required feature level needs to be delcared in app description.
	// All apps are assumed to support 9.1 unless otherwise stated.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	// Create the DX11 API device object, and get a corresponding context.
	ComPtr<ID3D11Device> device;

	DX::ThrowIfFailed(
		D3D11CreateDevice(
		nullptr, // Use default adapter.
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		creationFlags,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&device,
		&d3dFeatureLevel,
		&this->d3dContext)
		);

	DX::ThrowIfFailed(
		device.As(&this->d3dDevice)
		);
}

void RenderSystem::CreateD2DDevice()
{
	// Create factory for creating the Direct2D device.
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

	ComPtr<ID2D1Factory2> d2dFactory;
	DX::ThrowIfFailed(
		D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory1),
		&options,
		&d2dFactory)
		);

	// Obtain the underlying DXGI device of the Direct3D device.
	DX::ThrowIfFailed(
		this->d3dDevice.As(&this->dxgiDevice)
		);

	// Obtain the Direct2D device and device context for 2D rendering.
	ComPtr<ID2D1Device>	d2dDevice;
	DX::ThrowIfFailed(
		d2dFactory->CreateDevice(this->dxgiDevice.Get(), &d2dDevice)
		);

	DX::ThrowIfFailed(
		d2dDevice->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		&this->d2dContext
		)
		);
}

void RenderSystem::CreateSwapChain()
{
	// Allocate swap chain descriptor.
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
	swapChainDesc.Width = 0;										// Use automatic sizing.
	swapChainDesc.Height = 0;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;				// Most common swap chain format.
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;								// Don't use multi-sampling.
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = swapChainBufferCount;
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;	// All apps must use this swap effect.
	swapChainDesc.Flags = 0;

	// Identify the physical adapter (GPU or card) this device runs on.
	ComPtr<IDXGIAdapter> dxgiAdapter;
	DX::ThrowIfFailed(
		this->dxgiDevice->GetAdapter(&dxgiAdapter)
		);

	// Get the factory object that created the DXGI device to ensure the swap chain is created on the same adapter.
	ComPtr<IDXGIFactory2> dxgiFactory;
	DX::ThrowIfFailed(
		dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory))
		);

	// Get the swap chain for this window from the DXGI factory.
	DX::ThrowIfFailed(
		dxgiFactory->CreateSwapChainForCoreWindow(
		this->d3dDevice.Get(),
		reinterpret_cast<IUnknown*>(this->window.Get()),
		&swapChainDesc,
		nullptr, // Allow on all displays.
		&this->dxgiSwapChain
		)
		);

	// Ensure that DXGI doesn't queue more than one frame at a time to minimize power consumption.
	DX::ThrowIfFailed(
		dxgiDevice->SetMaximumFrameLatency(1)
		);
}

void RenderSystem::SetRenderTarget()
{
	// Get a D2D surface from the DXGI back buffer to use as the D2D render target.
	ComPtr<IDXGISurface> dxgiBackBuffer;
	DX::ThrowIfFailed(
		this->dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer))
		);

	// Create ID2D1Bitmap from the back buffer.
	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),  // Pixel format the Direct3D device and the DXGI device use.
		-1.0f,
		-1.0f
		);

	DX::ThrowIfFailed(
		this->d2dContext->CreateBitmapFromDxgiSurface(
		dxgiBackBuffer.Get(),
		&bitmapProperties,
		&this->d2dTargetBitmap
		)
		);

	// Set the Direct2D render target.
	this->d2dContext->SetTarget(this->d2dTargetBitmap.Get());

	// Grayscale text anti-aliasing is recommended for all Windows Store apps.
	this->d2dContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
}

void RenderSystem::Update(DX::StepTimer const& timer)
{
	// Early out if there's no window to render to.
	if (this->window == nullptr)
	{
		return;
	}

	// Create solid blue brush.
	ComPtr<ID2D1SolidColorBrush> pBlackBrush;
	DX::ThrowIfFailed(
		this->d2dContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Blue),
		&pBlackBrush
		)
		);

	// Draw rectangle.
	this->d2dContext->BeginDraw();

	this->d2dContext->DrawRectangle(
		D2D1::RectF(
		100.0f,
		100.0f,
		200.0f,
		200.0f),
		pBlackBrush.Get());

	DX::ThrowIfFailed(
		this->d2dContext->EndDraw()
		);

	// Present result.
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	DX::ThrowIfFailed(
		this->dxgiSwapChain->Present(1, 0)
	);
}

DXGI_MODE_ROTATION RenderSystem::ComputeDisplayRotation(DisplayOrientations nativeOrientation, DisplayOrientations currentOrientation)
{
	DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;

	// NativeOrientation can only be Landscape or Portrait even though the DisplayOrientations enum has other values.
	switch (nativeOrientation)
	{
	case Windows::Graphics::Display::DisplayOrientations::Landscape:
		switch (currentOrientation)
		{
		case Windows::Graphics::Display::DisplayOrientations::Landscape:
			rotation = DXGI_MODE_ROTATION_IDENTITY;
			break;

		case Windows::Graphics::Display::DisplayOrientations::Portrait:
			rotation = DXGI_MODE_ROTATION_ROTATE270;
			break;

		case Windows::Graphics::Display::DisplayOrientations::LandscapeFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE180;
			break;

		case Windows::Graphics::Display::DisplayOrientations::PortraitFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE90;
			break;
		}
		break;

	case Windows::Graphics::Display::DisplayOrientations::Portrait:
		switch (currentOrientation)
		{
		case Windows::Graphics::Display::DisplayOrientations::Landscape:
			rotation = DXGI_MODE_ROTATION_ROTATE90;
			break;

		case Windows::Graphics::Display::DisplayOrientations::Portrait:
			rotation = DXGI_MODE_ROTATION_IDENTITY;
			break;

		case Windows::Graphics::Display::DisplayOrientations::LandscapeFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE270;
			break;

		case Windows::Graphics::Display::DisplayOrientations::PortraitFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE180;
			break;
		}
		break;
	}
	return rotation;
}

void RenderSystem::OnDeviceLost()
{
	throw ref new Platform::FailureException();
}
