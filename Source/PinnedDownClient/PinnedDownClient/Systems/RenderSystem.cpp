
#include "pch.h"
#include "Core\Event.h"
#include "Core\EventManager.h"
#include "Systems\RenderSystem.h"

#include "Events\GraphicsDeviceLostEvent.h"
#include "Events\GraphicsDeviceRestoredEvent.h"
#include "Events\RenderTargetChangedEvent.h"

#include "Rendering\TextData.h"

using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Core::Resources;

RenderSystem::RenderSystem()
{
}

void RenderSystem::InitSystem(std::shared_ptr<EventManager> eventManager, std::shared_ptr<ResourceManager> resourceManager)
{
	GameSystem::InitSystem(eventManager, resourceManager);

	eventManager->AddListener(std::shared_ptr<IEventListener>(this), AppWindowChangedEvent::AppWindowChangedEventType);
	eventManager->AddListener(std::shared_ptr<IEventListener>(this), AppSuspendingEvent::AppSuspendingEventType);
	eventManager->AddListener(std::shared_ptr<IEventListener>(this), AppWindowSizeChangedEvent::AppWindowSizeChangedEventType);
	eventManager->AddListener(std::shared_ptr<IEventListener>(this), DisplayDpiChangedEvent::DisplayDpiChangedEventType);
	eventManager->AddListener(std::shared_ptr<IEventListener>(this), DisplayOrientationChangedEvent::DisplayOrientationChangedEventType);
	eventManager->AddListener(std::shared_ptr<IEventListener>(this), DisplayContentsInvalidatedEvent::DisplayContentsInvalidatedEventType);
	eventManager->AddListener(std::shared_ptr<IEventListener>(this), PointerMovedEvent::PointerMovedEventType);

	// Create devices.
	this->CreateD3DDevice();
	this->CreateD2DDevice();
	this->CreateDWriteFactory();
}

void RenderSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == AppWindowChangedEvent::AppWindowChangedEventType)
	{
		AppWindowChangedEvent appWindowChangedEvent = static_cast<AppWindowChangedEvent&>(newEvent);
		this->OnAppWindowChanged(appWindowChangedEvent);
	}
	else if (newEvent.GetEventType() == AppSuspendingEvent::AppSuspendingEventType)
	{
		this->OnAppSuspending();
	}
	else if (newEvent.GetEventType() == AppWindowSizeChangedEvent::AppWindowSizeChangedEventType)
	{
		AppWindowSizeChangedEvent appWindowSizeChangedEvent = static_cast<AppWindowSizeChangedEvent&>(newEvent);
		this->OnAppWindowSizeChanged(appWindowSizeChangedEvent);
	}
	else if (newEvent.GetEventType() == DisplayDpiChangedEvent::DisplayDpiChangedEventType)
	{
		DisplayDpiChangedEvent displayDpiChangedEvent = static_cast<DisplayDpiChangedEvent&>(newEvent);
		this->OnDisplayDpiChanged(displayDpiChangedEvent);
	}
	else if (newEvent.GetEventType() == DisplayOrientationChangedEvent::DisplayOrientationChangedEventType)
	{
		DisplayOrientationChangedEvent displayOrientationChangedEvent = static_cast<DisplayOrientationChangedEvent&>(newEvent);
		this->OnDisplayOrientationChanged(displayOrientationChangedEvent);
	}
	else if (newEvent.GetEventType() == DisplayContentsInvalidatedEvent::DisplayContentsInvalidatedEventType)
	{
		this->OnDisplayContentsInvalidated();
	}
	else if (newEvent.GetEventType() == PointerMovedEvent::PointerMovedEventType)
	{
		PointerMovedEvent pointerMovedEvent = static_cast<PointerMovedEvent&>(newEvent);
		this->OnPointerMoved(pointerMovedEvent);
	}
}

void RenderSystem::OnAppWindowChanged(AppWindowChangedEvent appWindowChangedEvent)
{
	this->window = appWindowChangedEvent.appWindow;
	
	// Store current window data for future updates.
	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	this->logicalWindowWidth = this->window->Bounds.Width;
	this->logicalWindowHeight = this->window->Bounds.Height;
	this->logicalDpi = currentDisplayInformation->LogicalDpi;
	this->displayOrientation = currentDisplayInformation->CurrentOrientation;

	// Set render target.
	this->CreateWindowSizeDependentResources();
}

void RenderSystem::OnAppSuspending()
{
	// Provides a hint to the driver that the app is entering an idle state and that temporary buffers can be reclaimed for use by other apps.
	ComPtr<IDXGIDevice3> dxgiDevice;
	this->d3dDevice.As(&dxgiDevice);

	dxgiDevice->Trim();
}

void RenderSystem::OnAppWindowSizeChanged(AppWindowSizeChangedEvent appWindowSizeChangedEvent)
{
	this->logicalWindowWidth = appWindowSizeChangedEvent.width;
	this->logicalWindowHeight = appWindowSizeChangedEvent.height;

	this->CreateWindowSizeDependentResources();
}

void RenderSystem::OnDisplayDpiChanged(DisplayDpiChangedEvent displayDpiChangedEvent)
{
	this->logicalDpi = displayDpiChangedEvent.logicalDpi;

	this->CreateWindowSizeDependentResources();
}

void RenderSystem::OnDisplayOrientationChanged(DisplayOrientationChangedEvent displayOrientationChangedEvent)
{
	this->displayOrientation = displayOrientationChangedEvent.orientation;

	this->CreateWindowSizeDependentResources();
}

void RenderSystem::OnDisplayContentsInvalidated()
{
	// The D3D Device is no longer valid if the default adapter change since the device
	// was created or if the device has been removed.

	// First, get the information for the default adapter from when the device was created.
	ComPtr<IDXGIAdapter> deviceAdapter;
	DX::ThrowIfFailed(this->dxgiDevice->GetAdapter(&deviceAdapter));

	ComPtr<IDXGIFactory2> deviceFactory;
	DX::ThrowIfFailed(deviceAdapter->GetParent(IID_PPV_ARGS(&deviceFactory)));

	ComPtr<IDXGIAdapter1> previousDefaultAdapter;
	DX::ThrowIfFailed(deviceFactory->EnumAdapters1(0, &previousDefaultAdapter));

	DXGI_ADAPTER_DESC previousDesc;
	DX::ThrowIfFailed(previousDefaultAdapter->GetDesc(&previousDesc));

	// Next, get the information for the current default adapter.
	ComPtr<IDXGIFactory2> currentFactory;
	DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&currentFactory)));

	ComPtr<IDXGIAdapter1> currentDefaultAdapter;
	DX::ThrowIfFailed(currentFactory->EnumAdapters1(0, &currentDefaultAdapter));

	DXGI_ADAPTER_DESC currentDesc;
	DX::ThrowIfFailed(currentDefaultAdapter->GetDesc(&currentDesc));

	// If the adapter LUIDs don't match, or if the device reports that it has been removed,
	// a new D3D device must be created.
	if (previousDesc.AdapterLuid.LowPart != currentDesc.AdapterLuid.LowPart ||
		previousDesc.AdapterLuid.HighPart != currentDesc.AdapterLuid.HighPart ||
		FAILED(this->d3dDevice->GetDeviceRemovedReason()))
	{
		// Release references to resources related to the old device.
		this->dxgiDevice = nullptr;
		deviceAdapter = nullptr;
		deviceFactory = nullptr;
		previousDefaultAdapter = nullptr;

		// Create a new device and swap chain.
		this->OnDeviceLost();
	}
}

void RenderSystem::OnPointerMoved(PointerMovedEvent pointerMovedEvent)
{
	this->pointerId = pointerMovedEvent.pointerId;
	this->pointerPositionX = pointerMovedEvent.positionX;
	this->pointerPositionY = pointerMovedEvent.positionY;
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

	// Create drawing state block used for pushing and popping context tranformations.
	DX::ThrowIfFailed(
		d2dFactory->CreateDrawingStateBlock(&this->drawingStateBlock)
		);
}

void RenderSystem::CreateDWriteFactory()
{
	DX::ThrowIfFailed(
		DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory2),
		&this->writeFactory)
		);

	// Create text format for the default font and size.
	DX::ThrowIfFailed(
		writeFactory->CreateTextFormat(
		L"Arial",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		12.0f,
		L"en-US",
		&this->textFormat)
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

	// Create DirectWrite brushes.
	this->CreateBrushes();

	// Notify listeners.
	auto renderTargetChangedEvent = std::shared_ptr<Events::RenderTargetChangedEvent>(new Events::RenderTargetChangedEvent(this->d2dContext));
	this->eventManager->RaiseEvent(renderTargetChangedEvent);

	// Load resources.
	this->LoadResources();
}

void RenderSystem::CreateBrushes()
{
	// Create brush for font rendering.
	if (this->redBrush != nullptr)
	{
		this->redBrush.Reset();
	}

	DX::ThrowIfFailed(
		this->d2dContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red),
		&this->redBrush)
		);
}

void RenderSystem::LoadResources()
{
	this->resourceManager->LoadBitmapFromFile(
		this->d2dContext.Get(),
		L"Logo",
		L"Assets/Logo.png"
		);

	this->resourceManager->LoadBitmapFromFile(
		this->d2dContext.Get(),
		L"SmallLogo",
		L"Assets/SmallLogo.png"
		);

	this->resourceManager->LoadBitmapFromFile(
		this->d2dContext.Get(),
		L"SplashScreen",
		L"Assets/SplashScreen.png"
		);

	this->resourceManager->LoadBitmapFromFile(
		this->d2dContext.Get(),
		L"StoreLogo",
		L"Assets/StoreLogo.png"
		);
}

void RenderSystem::Render()
{
	// Early out if there's no window to render to.
	if (this->window == nullptr)
	{
		return;
	}

	// Clear the back buffer and depth stencil view.
	this->d3dContext->ClearRenderTargetView(this->d3dRenderTargetView.Get(), DirectX::Colors::CornflowerBlue);
	
	// Save drawing state.
	this->d2dContext->SaveDrawingState(this->drawingStateBlock.Get());

	this->d2dContext->BeginDraw();

	// Draw rectangle.
	this->d2dContext->DrawRectangle(
		D2D1::RectF(
		100.0f,
		100.0f,
		200.0f,
		200.0f),
		this->redBrush.Get());

	// Debug text to draw.
	Rendering::TextData textData = Rendering::TextData();
	textData.Text = L"Pointer Id: " + std::to_wstring(this->pointerId) + L"\nPointer X: " + std::to_wstring(this->pointerPositionX) + L"\nPointer Y: " + std::to_wstring(this->pointerPositionY);
	textData.X = 250;
	textData.Y = 200;
	textData.Alignment = DWRITE_TEXT_ALIGNMENT_LEADING;

	// Set text alignment.
	DX::ThrowIfFailed(
		this->textFormat->SetTextAlignment(textData.Alignment)
		);

	// TODO: Adjust translation via text metrics depending on alignment.
	D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(textData.X, textData.Y);
	this->d2dContext->SetTransform(screenTranslation);

	// Create final text layout for drawing.
	Microsoft::WRL::ComPtr<IDWriteTextLayout> textLayout;

	DX::ThrowIfFailed(
		this->writeFactory->CreateTextLayout(
		textData.Text.c_str(),
		(uint32)textData.Text.length(),
		this->textFormat.Get(),
		500.0f, // Max width.
		500.0f, // Max height.
		&textLayout)
		);

	DWRITE_TEXT_METRICS metrics;
	DX::ThrowIfFailed(
		textLayout->GetMetrics(&metrics)
		);

	// Text metrics, such as line height, can be accessed here: metrics.height

	// Draw text.
	this->d2dContext->DrawTextLayout(
		D2D1::Point2F(0.f, 0.f),
		textLayout.Get(),
		this->redBrush.Get()
		);

	// Draw bitmaps.
	this->d2dContext->SetTransform(D2D1::Matrix3x2F::Translation(400, 400));

	BitmapResourceHandle & logoBitmap = this->resourceManager->GetResource<BitmapResourceHandle>(L"Logo");
	this->DrawBitmap(logoBitmap);
	BitmapResourceHandle & smallLogoBitmap = this->resourceManager->GetResource<BitmapResourceHandle>(L"SmallLogo");
	this->DrawBitmap(smallLogoBitmap);
	BitmapResourceHandle & splashScreenBitmap = this->resourceManager->GetResource<BitmapResourceHandle>(L"SplashScreen");
	this->DrawBitmap(splashScreenBitmap);
	BitmapResourceHandle & storeLogoBitmap = this->resourceManager->GetResource<BitmapResourceHandle>(L"StoreLogo");
	this->DrawBitmap(storeLogoBitmap);

	DX::ThrowIfFailed(
		this->d2dContext->EndDraw()
		);

	// Restore drawing state.
	this->d2dContext->RestoreDrawingState(this->drawingStateBlock.Get());

	// Present result.
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT hr = this->dxgiSwapChain->Present(1, 0);

	// If the device was removed either by a disconnection or a driver upgrade, we
	// must recreate all device resources.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		this->OnDeviceLost();
	}
	else
	{
		DX::ThrowIfFailed(hr);
	}
}

DXGI_MODE_ROTATION RenderSystem::ComputeDisplayRotation(DisplayOrientations nativeOrientation, DisplayOrientations currentOrientation)
{
	DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;

	// NativeOrientation can only be Landscape or Portrait even though the DisplayOrientations enum has other values.
	switch (nativeOrientation)
	{
	case DisplayOrientations::Landscape:
		switch (currentOrientation)
		{
		case DisplayOrientations::Landscape:
			rotation = DXGI_MODE_ROTATION_IDENTITY;
			break;

		case DisplayOrientations::Portrait:
			rotation = DXGI_MODE_ROTATION_ROTATE270;
			break;

		case DisplayOrientations::LandscapeFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE180;
			break;

		case DisplayOrientations::PortraitFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE90;
			break;
		}
		break;

	case DisplayOrientations::Portrait:
		switch (currentOrientation)
		{
		case DisplayOrientations::Landscape:
			rotation = DXGI_MODE_ROTATION_ROTATE90;
			break;

		case DisplayOrientations::Portrait:
			rotation = DXGI_MODE_ROTATION_IDENTITY;
			break;

		case DisplayOrientations::LandscapeFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE270;
			break;

		case DisplayOrientations::PortraitFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE180;
			break;
		}
		break;
	}
	return rotation;
}

void RenderSystem::CreateWindowSizeDependentResources()
{
	this->d2dContext->SetDpi(this->logicalDpi, this->logicalDpi);

	// Clear the previous window size specific context.
	this->d2dContext->SetTarget(nullptr);
	this->d2dTargetBitmap = nullptr;
	this->d3dContext->Flush();

	// Calculate the necessary render target size in pixels (for CoreWindow).
	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	float currentWidth = DX::ConvertDipsToPixels(this->logicalWindowWidth, this->logicalDpi);
	float currentHeight = DX::ConvertDipsToPixels(this->logicalWindowWidth, this->logicalDpi);

	// Prevent zero size DirectX content from being created.
	currentWidth = max(currentWidth, 1);
	currentHeight = max(currentHeight, 1);

	// The width and height of the swap chain must be based on the window's
	// natively-oriented width and height. If the window is not in the native
	// orientation, the dimensions must be reversed.
	DXGI_MODE_ROTATION displayRotation = ComputeDisplayRotation(currentDisplayInformation->NativeOrientation, this->displayOrientation);

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

	// Create a render target view of the swap chain back buffer.
	ComPtr<ID3D11Texture2D> backBuffer;
	DX::ThrowIfFailed(
		this->dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))
		);

	DX::ThrowIfFailed(
		this->d3dDevice->CreateRenderTargetView(
		backBuffer.Get(),
		nullptr,
		&this->d3dRenderTargetView
		)
		);

	// Set the 3D rendering viewport to target the entire window.
	this->d3dScreenViewport = CD3D11_VIEWPORT(
		0.0f,
		0.0f,
		newWidth,
		newHeight
		);

	this->d3dContext->RSSetViewports(1, &this->d3dScreenViewport);

	// Create a Direct2D target bitmap associated with the
	// swap chain back buffer and set it as the current target.
	this->SetRenderTarget();
}

void RenderSystem::OnDeviceLost()
{
	// Crash on unnatural lost devices.
	HRESULT reason = this->d3dDevice->GetDeviceRemovedReason();

	if (reason == DXGI_ERROR_DEVICE_HUNG || reason == DXGI_ERROR_DEVICE_RESET || DXGI_ERROR_INVALID_CALL)
	{
		throw Platform::Exception::CreateException(reason);
	}

	// All references to the swap chain must be released before a new one can be created.
	this->dxgiSwapChain = nullptr;

	// Notify the renderers that device resources need to be released.
	// This ensures all references to the existing swap chain are released so that a new one can be created.
	auto graphicsDeviceLostEvent = std::shared_ptr<Events::GraphicsDeviceLostEvent>(new Events::GraphicsDeviceLostEvent());
	this->eventManager->RaiseEvent(graphicsDeviceLostEvent);

	// Create the new device and swap chain.
	this->CreateD3DDevice();
	this->CreateD2DDevice();
	this->CreateWindowSizeDependentResources();

	// Notify the renderers that resources can now be created again.
	auto graphicsDeviceRestoredEvent = std::shared_ptr<Events::GraphicsDeviceRestoredEvent>(new Events::GraphicsDeviceRestoredEvent());
	this->eventManager->RaiseEvent(graphicsDeviceRestoredEvent);
}

void RenderSystem::DrawBitmap(BitmapResourceHandle & bitmapHandle)
{
	D2D1_SIZE_F size = bitmapHandle.bitmap->GetSize();

	this->d2dContext->DrawBitmap(
		bitmapHandle.bitmap,
		D2D1::RectF(
		0,
		0,
		size.width,
		size.height)
		);
}