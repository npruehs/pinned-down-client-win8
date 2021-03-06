
#include "pch.h"
#include "Event.h"
#include "EventManager.h"
#include "Systems\RenderSystem.h"
#include "Math\Vector2F.h"
#include "Util\DirectXUtils.h"

#include "Components\ColorComponent.h"
#include "Components\FontComponent.h"
#include "Components\ScreenPositionComponent.h"
#include "Components\SpriteComponent.h"
#include "Components\TextAlignmentComponent.h"
#include "Components\TextComponent.h"
#include "Components\UIWidgetComponent.h"
#include "Components\VisibilityComponent.h"

#include "Events\GraphicsDeviceLostEvent.h"
#include "Events\GraphicsDeviceRestoredEvent.h"
#include "Events\RenderTargetChangedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Components;
using namespace PinnedDownClient::Math;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Resources;
using namespace PinnedDownClient::Util;


RenderSystem::RenderSystem()
{
}

void RenderSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, AppWindowChangedEvent::AppWindowChangedEventType);
	this->game->eventManager->AddListener(this, AppSuspendingEvent::AppSuspendingEventType);
	this->game->eventManager->AddListener(this, AppWindowSizeChangedEvent::AppWindowSizeChangedEventType);
	this->game->eventManager->AddListener(this, DisplayDpiChangedEvent::DisplayDpiChangedEventType);
	this->game->eventManager->AddListener(this, DisplayOrientationChangedEvent::DisplayOrientationChangedEventType);
	this->game->eventManager->AddListener(this, DisplayContentsInvalidatedEvent::DisplayContentsInvalidatedEventType);
	this->game->eventManager->AddListener(this, EntityInitializedEvent::EntityInitializedEventType);
	this->game->eventManager->AddListener(this, EntityRemovedEvent::EntityRemovedEventType);

	// Create devices.
	this->CreateD3DDevice();
	this->CreateD2DDevice();
	this->CreateDWriteFactory();
}

void RenderSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(AppWindowChangedEvent);
	CALL_EVENT_HANDLER(AppSuspendingEvent);
	CALL_EVENT_HANDLER(AppWindowSizeChangedEvent);
	CALL_EVENT_HANDLER(DisplayDpiChangedEvent);
	CALL_EVENT_HANDLER(DisplayOrientationChangedEvent);
	CALL_EVENT_HANDLER(DisplayContentsInvalidatedEvent);
	CALL_EVENT_HANDLER(EntityInitializedEvent);
	CALL_EVENT_HANDLER(EntityRemovedEvent);
}

EVENT_HANDLER_DEFINITION(RenderSystem, AppWindowChangedEvent)
{
	this->window = data.appWindow;
	
	// Store current window data for future updates.
	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	this->logicalWindowSize = Vector2F(this->window->Bounds.Width, this->window->Bounds.Height);
	this->logicalDpi = currentDisplayInformation->LogicalDpi;
	this->displayOrientation = currentDisplayInformation->CurrentOrientation;

	// Set render target.
	this->CreateWindowSizeDependentResources();
}

EVENT_HANDLER_DEFINITION(RenderSystem, AppSuspendingEvent)
{
	// Provides a hint to the driver that the app is entering an idle state and that temporary buffers can be reclaimed for use by other apps.
	ComPtr<IDXGIDevice3> dxgiDevice;
	this->d3dDevice.As(&dxgiDevice);

	dxgiDevice->Trim();
}

EVENT_HANDLER_DEFINITION(RenderSystem, AppWindowSizeChangedEvent)
{
	this->logicalWindowSize = data.size;

	this->CreateWindowSizeDependentResources();
}

EVENT_HANDLER_DEFINITION(RenderSystem, DisplayDpiChangedEvent)
{
	this->logicalDpi = data.logicalDpi;

	this->CreateWindowSizeDependentResources();
}

EVENT_HANDLER_DEFINITION(RenderSystem, DisplayOrientationChangedEvent)
{
	this->displayOrientation = data.orientation;

	this->CreateWindowSizeDependentResources();
}

EVENT_HANDLER_DEFINITION(RenderSystem, DisplayContentsInvalidatedEvent)
{
	// The D3D Device is no longer valid if the default adapter change since the device
	// was created or if the device has been removed.

	// First, get the information for the default adapter from when the device was created.
	ComPtr<IDXGIAdapter> deviceAdapter;
	ThrowIfFailed(this->dxgiDevice->GetAdapter(&deviceAdapter));

	ComPtr<IDXGIFactory2> deviceFactory;
	ThrowIfFailed(deviceAdapter->GetParent(IID_PPV_ARGS(&deviceFactory)));

	ComPtr<IDXGIAdapter1> previousDefaultAdapter;
	ThrowIfFailed(deviceFactory->EnumAdapters1(0, &previousDefaultAdapter));

	DXGI_ADAPTER_DESC previousDesc;
	ThrowIfFailed(previousDefaultAdapter->GetDesc(&previousDesc));

	// Next, get the information for the current default adapter.
	ComPtr<IDXGIFactory2> currentFactory;
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&currentFactory)));

	ComPtr<IDXGIAdapter1> currentDefaultAdapter;
	ThrowIfFailed(currentFactory->EnumAdapters1(0, &currentDefaultAdapter));

	DXGI_ADAPTER_DESC currentDesc;
	ThrowIfFailed(currentDefaultAdapter->GetDesc(&currentDesc));

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

EVENT_HANDLER_DEFINITION(RenderSystem, EntityInitializedEvent)
{
	auto entity = data.entity;

	auto boundsComponent = this->game->entityManager->GetComponent<BoundsComponent>(entity, BoundsComponent::BoundsComponentType);
	auto colorComponent = this->game->entityManager->GetComponent<ColorComponent>(entity, ColorComponent::ColorComponentType);
	auto depthComponent = this->game->entityManager->GetComponent<DepthComponent>(entity, DepthComponent::DepthComponentType);
	auto fontComponent = this->game->entityManager->GetComponent<FontComponent>(entity, FontComponent::FontComponentType);
	auto screenPositionComponent = this->game->entityManager->GetComponent<ScreenPositionComponent>(entity, ScreenPositionComponent::ScreenPositionComponentType);
	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(entity, TextComponent::TextComponentType);
	auto textAlignmentComponent = this->game->entityManager->GetComponent<TextAlignmentComponent>(entity, TextAlignmentComponent::TextAlignmentComponentType);
	auto spriteComponent = this->game->entityManager->GetComponent<SpriteComponent>(entity, SpriteComponent::SpriteComponentType);
	auto widgetComponent = this->game->entityManager->GetComponent<UIWidgetComponent>(entity, UIWidgetComponent::UIWidgetComponentType);
	auto visibilityComponent = this->game->entityManager->GetComponent<VisibilityComponent>(entity, VisibilityComponent::VisibilityComponentType);

	if (boundsComponent != nullptr
		&& colorComponent != nullptr
		&& depthComponent != nullptr
		&& fontComponent != nullptr
		&& screenPositionComponent != nullptr
		&& textComponent != nullptr
		&& textAlignmentComponent != nullptr
		&& visibilityComponent != nullptr)
	{
		// Add label.
		std::shared_ptr<UI::Label> label = std::make_shared<UI::Label>();
		label->entity = entity;
		label->boundsComponent = boundsComponent;
		label->colorComponent = colorComponent;
		label->depthComponent = depthComponent;
		label->fontComponent = fontComponent;
		label->screenPositionComponent = screenPositionComponent;
		label->textAlignmentComponent = textAlignmentComponent;
		label->textComponent = textComponent;
		label->visibilityComponent = visibilityComponent;

		if (widgetComponent->panel != INVALID_ENTITY_ID)
		{
			label->panelDepthComponent = this->game->entityManager->GetComponent<DepthComponent>(widgetComponent->panel, DepthComponent::DepthComponentType);
			label->panelVisibilityComponent = this->game->entityManager->GetComponent<VisibilityComponent>(widgetComponent->panel, VisibilityComponent::VisibilityComponentType);
		}

		this->renderables.push_back(label);
	}

	if (boundsComponent != nullptr
		&& depthComponent != nullptr
		&& screenPositionComponent != nullptr
		&& spriteComponent != nullptr
		&& widgetComponent != nullptr
		&& visibilityComponent != nullptr)
	{
		// Add sprite.
		std::shared_ptr<UI::Sprite> sprite = std::make_shared<UI::Sprite>();
		sprite->entity = entity;
		sprite->boundsComponent = boundsComponent;
		sprite->depthComponent = depthComponent;
		sprite->screenPositionComponent = screenPositionComponent;
		sprite->spriteComponent = spriteComponent;
		sprite->visibilityComponent = visibilityComponent;

		if (widgetComponent->panel != INVALID_ENTITY_ID)
		{
			sprite->panelDepthComponent = this->game->entityManager->GetComponent<DepthComponent>(widgetComponent->panel, DepthComponent::DepthComponentType);
			sprite->panelVisibilityComponent = this->game->entityManager->GetComponent<VisibilityComponent>(widgetComponent->panel, VisibilityComponent::VisibilityComponentType);
		}

		this->renderables.push_back(sprite);
	}
}

EVENT_HANDLER_DEFINITION(RenderSystem, EntityRemovedEvent)
{
	auto entity = data.entity;

	// Remove renderable.
	for (std::list<std::shared_ptr<Rendering::IRenderable>>::iterator iterator = this->renderables.begin(); iterator != this->renderables.end(); ++iterator)
	{
		std::shared_ptr<Rendering::IRenderable>& renderable = *iterator;

		if (renderable->GetEntity() == entity)
		{
			this->renderables.erase(iterator);
			return;
		}
	}
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

	ThrowIfFailed(
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

	ThrowIfFailed(
		device.As(&this->d3dDevice)
		);
}

void RenderSystem::CreateD2DDevice()
{
	// Create factory for creating the Direct2D device.
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

	ComPtr<ID2D1Factory2> d2dFactory;
	ThrowIfFailed(
		D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory1),
		&options,
		&d2dFactory)
		);

	// Obtain the underlying DXGI device of the Direct3D device.
	ThrowIfFailed(
		this->d3dDevice.As(&this->dxgiDevice)
		);

	// Obtain the Direct2D device and device context for 2D rendering.
	ComPtr<ID2D1Device>	d2dDevice;
	ThrowIfFailed(
		d2dFactory->CreateDevice(this->dxgiDevice.Get(), &d2dDevice)
		);

	ThrowIfFailed(
		d2dDevice->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		&this->d2dContext
		)
		);

	// Create drawing state block used for pushing and popping context tranformations.
	ThrowIfFailed(
		d2dFactory->CreateDrawingStateBlock(&this->drawingStateBlock)
		);
}

void RenderSystem::CreateDWriteFactory()
{
	ThrowIfFailed(
		DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory2),
		&this->writeFactory)
		);
}

void RenderSystem::CreateSwapChain()
{
	this->windowScale = Vector2F(1.0f, 1.0f);

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
	ThrowIfFailed(
		this->dxgiDevice->GetAdapter(&dxgiAdapter)
		);

	// Get the factory object that created the DXGI device to ensure the swap chain is created on the same adapter.
	ComPtr<IDXGIFactory2> dxgiFactory;
	ThrowIfFailed(
		dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory))
		);

	// Get the swap chain for this window from the DXGI factory.
	ComPtr<IDXGISwapChain1> swapChain;
	ThrowIfFailed(
		dxgiFactory->CreateSwapChainForCoreWindow(
		this->d3dDevice.Get(),
		reinterpret_cast<IUnknown*>(this->window.Get()),
		&swapChainDesc,
		nullptr, // Allow on all displays.
		&swapChain
		)
		);

	ThrowIfFailed(
		swapChain.As(&this->dxgiSwapChain)
		);

	// Change the region of the swap chain that will be presented to the screen.
	ThrowIfFailed(
		this->dxgiSwapChain->SetSourceSize(
		static_cast<UINT>(this->logicalWindowSize.x * this->windowScale.x + 0.5f),
		static_cast<UINT>(this->logicalWindowSize.y * this->windowScale.y + 0.5f)
		)
		);

	// Ensure that DXGI doesn't queue more than one frame at a time to minimize power consumption.
	ThrowIfFailed(
		dxgiDevice->SetMaximumFrameLatency(1)
		);
}

void RenderSystem::SetRenderTarget()
{
	// Get a D2D surface from the DXGI back buffer to use as the D2D render target.
	ComPtr<IDXGISurface> dxgiBackBuffer;
	ThrowIfFailed(
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

	ThrowIfFailed(
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

	// Notify listeners.
	auto renderTargetChangedEvent = std::make_shared<Events::RenderTargetChangedEvent>(this->d2dContext);
	this->game->eventManager->RaiseEvent(renderTargetChangedEvent);
}

void RenderSystem::Render()
{
	// Early out if there's no window to render to.
	if (this->window == nullptr)
	{
		return;
	}

	// Clear the back buffer and depth stencil view.
	this->d3dContext->ClearRenderTargetView(this->d3dRenderTargetView.Get(), DirectX::Colors::Black);
	
	// Save drawing state.
	this->d2dContext->SaveDrawingState(this->drawingStateBlock.Get());

	this->d2dContext->BeginDraw();

	// Draw renderables.
	this->renderables.sort(&RenderSystem::SortByDepth);

	for (std::list<std::shared_ptr<Rendering::IRenderable>>::iterator iterator = this->renderables.begin(); iterator != this->renderables.end(); ++iterator)
	{
		std::shared_ptr<Rendering::IRenderable>& renderable = *iterator;

		// Visibility check.
		if (!renderable->visibilityComponent->visible
			|| (renderable->panelVisibilityComponent != nullptr && !renderable->panelVisibilityComponent->visible)
			|| !renderable->screenPositionComponent->positionValid)
		{
			continue;
		}

		// Translate context.
		float x = renderable->screenPositionComponent->position.x;
		float y = renderable->screenPositionComponent->position.y;

		D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(x, y);
		this->d2dContext->SetTransform(screenTranslation);

		// Render item.
		std::shared_ptr<UI::Sprite> sprite = std::dynamic_pointer_cast<UI::Sprite>(*iterator);
		std::shared_ptr<UI::Label> label = std::dynamic_pointer_cast<UI::Label>(*iterator);
		
		if (sprite != nullptr)
		{
			// Draw sprite.
			this->DrawSprite(sprite);
		}
		else if (label != nullptr)
		{
			// Draw text.
			this->DrawLabel(label);
		}
	}

	ThrowIfFailed(
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
		ThrowIfFailed(hr);
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
	this->d3dRenderTargetView = nullptr;
	this->d2dContext->SetTarget(nullptr);
	this->d2dTargetBitmap = nullptr;
	this->d3dContext->Flush();

	// Calculate the necessary render target size in pixels (for CoreWindow).
	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	float currentWidth = ConvertDipsToPixels(this->logicalWindowSize.x, this->logicalDpi);
	float currentHeight = ConvertDipsToPixels(this->logicalWindowSize.y, this->logicalDpi);

	// Prevent zero size DirectX content from being created.
	currentWidth = max(currentWidth, 1);
	currentHeight = max(currentHeight, 1);

	// The width and height of the swap chain must be based on the window's
	// natively-oriented width and height. If the window is not in the native
	// orientation, the dimensions must be reversed.
	DXGI_MODE_ROTATION displayRotation = this->ComputeDisplayRotation(currentDisplayInformation->NativeOrientation, this->displayOrientation);

	bool swapDimensions = displayRotation == DXGI_MODE_ROTATION_ROTATE90 || displayRotation == DXGI_MODE_ROTATION_ROTATE270;
	float newWidth = swapDimensions ? currentHeight : currentWidth;
	float newHeight = swapDimensions ? currentWidth : currentHeight;

	if (this->dxgiSwapChain != nullptr)
	{
		// If the swap chain already exists, resize it, preserving all other settings.
		HRESULT hr = this->dxgiSwapChain->ResizeBuffers(
			0,
			static_cast<UINT>(newWidth),
			static_cast<UINT>(newHeight),
			DXGI_FORMAT_UNKNOWN,
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
			ThrowIfFailed(hr);
		}
	}
	else
	{
		// Otherwise, create a new one using the same adapter as the existing Direct3D device.
		this->CreateSwapChain();
	}

	// Set the proper orientation for the swap chain.
	// TODO(np): Always set swap chain rotation?
	ThrowIfFailed(
		this->dxgiSwapChain->SetRotation(displayRotation)
		);

	// Create a render target view of the swap chain back buffer.
	ComPtr<ID3D11Texture2D> backBuffer;
	ThrowIfFailed(
		this->dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))
		);

	ThrowIfFailed(
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
		newWidth * this->windowScale.x + 0.5f,
		newHeight * this->windowScale.y + 0.5f
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
	auto graphicsDeviceLostEvent = std::make_shared<Events::GraphicsDeviceLostEvent>();
	this->game->eventManager->RaiseEvent(graphicsDeviceLostEvent);

	// Create the new device and swap chain.
	this->CreateD3DDevice();
	this->CreateD2DDevice();
	this->CreateWindowSizeDependentResources();

	// Notify the renderers that resources can now be created again.
	auto graphicsDeviceRestoredEvent = std::make_shared<Events::GraphicsDeviceRestoredEvent>();
	this->game->eventManager->RaiseEvent(graphicsDeviceRestoredEvent);
}

void RenderSystem::DrawSprite(std::shared_ptr<UI::Sprite> sprite)
{
	D2D1_SIZE_F size = sprite->spriteComponent->sprite->bitmap->GetSize();

	// Scale sprites, but preserve original aspect ratio.
	float spriteScaleX = this->logicalWindowSize.x / this->designResolution.x;
	float spriteWidth = size.width * spriteScaleX;
	float spriteHeight = size.height * spriteScaleX;

	sprite->boundsComponent->bounds = Vector2F(spriteWidth, spriteHeight);

	// Draw sprite.
	this->d2dContext->DrawBitmap(
		sprite->spriteComponent->sprite->bitmap,
		D2D1::RectF(
		0,
		0,
		sprite->boundsComponent->bounds.x,
		sprite->boundsComponent->bounds.y)
		);
}

void RenderSystem::DrawLabel(std::shared_ptr<UI::Label> label)
{
	float fontScale = this->logicalWindowSize.x / this->designResolution.x;

	// Create text format.
	ComPtr<IDWriteTextFormat> textFormat;

	ThrowIfFailed(
		this->writeFactory->CreateTextFormat(
		label->fontComponent->fontFamilyName.c_str(),
		NULL,
		label->fontComponent->fontWeight,
		label->fontComponent->fontStyle,
		label->fontComponent->fontStretch,
		label->fontComponent->fontSize * fontScale,
		L"en-US",
		&textFormat)
		);

	// Set text alignment.
	ThrowIfFailed(
		textFormat->SetTextAlignment(label->textAlignmentComponent->alignment)
		);

	// Create final text layout for drawing.
	Microsoft::WRL::ComPtr<IDWriteTextLayout> textLayout;

	ThrowIfFailed(
		this->writeFactory->CreateTextLayout(
		label->textComponent->text.c_str(),
		(uint32)label->textComponent->text.length(),
		textFormat.Get(),
		label->textComponent->maxWidth * this->logicalWindowSize.x, // Max width.
		0.0f, // Max height.
		&textLayout)
		);

	DWRITE_TEXT_METRICS metrics;
	ThrowIfFailed(
		textLayout->GetMetrics(&metrics)
		);

	// Update text bounds.
	label->boundsComponent->bounds = Vector2F(metrics.width, metrics.height);

	// Create brush for font rendering.
	ComPtr<ID2D1SolidColorBrush> textBrush;

	ThrowIfFailed(
		this->d2dContext->CreateSolidColorBrush(
		label->colorComponent->color,
		&textBrush)
		);

	// Draw text.
	this->d2dContext->DrawTextLayout(
		D2D1::Point2F(0.f, 0.f),
		textLayout.Get(),
		textBrush.Get()
		);
}


bool RenderSystem::SortByDepth(const std::shared_ptr<Rendering::IRenderable>& first, const std::shared_ptr<Rendering::IRenderable>& second)
{
	// Get widget depths.
	int firstDepth = first->depthComponent->depth;
	int secondDepth = second->depthComponent->depth;

	// Add panel depths.
	if (first->panelDepthComponent != nullptr)
	{
		firstDepth += first->panelDepthComponent->depth;
	}

	if (second->panelDepthComponent != nullptr)
	{
		secondDepth += second->panelDepthComponent->depth;
	}

	return firstDepth < secondDepth;
}