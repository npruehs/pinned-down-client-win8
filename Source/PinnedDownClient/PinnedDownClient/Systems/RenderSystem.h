#pragma once

#include "PinnedDownGame.h"
#include "IEventListener.h"
#include "GameSystem.h"
#include "Events\AppWindowChangedEvent.h"
#include "Events\AppSuspendingEvent.h"
#include "Events\AppWindowSizeChangedEvent.h"
#include "Events\DisplayDpiChangedEvent.h"
#include "Events\DisplayOrientationChangedEvent.h"
#include "Events\DisplayContentsInvalidatedEvent.h"
#include "EntityInitializedEvent.h"
#include "EntityRemovedEvent.h"
#include "Resources\BitmapResourceHandle.h"
#include "Math\Vector2F.h"
#include "Rendering\IRenderable.h"
#include "UI\Sprite.h"
#include "UI\Label.h"

using namespace Microsoft::WRL;
using namespace Windows::Graphics::Display;

using namespace PinnedDownCore;
using namespace PinnedDownClient::Math;
using namespace PinnedDownClient::Resources;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems::UI;

namespace PinnedDownClient
{
	namespace Systems
	{
		class RenderSystem : public GameSystem, public IEventListener
		{
		public:
			RenderSystem();

			void InitSystem(PinnedDownCore::Game* game);
			void Render();

		private:
			Platform::Agile<Windows::UI::Core::CoreWindow> window;

			// Direct3D hardware device for the default adapter.
			ComPtr<ID3D11Device2> d3dDevice;

			// Set of state and command buffers used to render to a target. Call methods on this context to set pipeline state and generate rendering commands.
			ComPtr<ID3D11DeviceContext> d3dContext;

			// Feature level of the current Direct3D device.
			D3D_FEATURE_LEVEL d3dFeatureLevel;

			// Render target view of the swap chain back buffer. Cleared every frame.
			ComPtr<ID3D11RenderTargetView> d3dRenderTargetView;

			// 3D rendering viewport targeting the entire window.
			D3D11_VIEWPORT d3dScreenViewport;

			// Underlying DirectX Graphics Infrastructure device of the Direct3D device. Used for retrieving the Direct2D device.
			ComPtr<IDXGIDevice3> dxgiDevice;

			// Swap chain presenting our results.
			ComPtr<IDXGISwapChain2> dxgiSwapChain;

			// Use double buffering to enable flip.
			unsigned int swapChainBufferCount = 2;

			// Set of state and command buffers used to render to a target. Call methods on this context to set pipeline state and generate rendering commands.
			ComPtr<ID2D1DeviceContext> d2dContext;

			// Anything rendered to this bitmap is rendered to the surface of the swap chain.
			ComPtr<ID2D1Bitmap1> d2dTargetBitmap;

			// Drawing state block used for pushing and popping context tranformations.
			ComPtr<ID2D1DrawingStateBlock> drawingStateBlock;

			// DirectWrite factory used to create text formats and layouts.
			ComPtr<IDWriteFactory2>	writeFactory;

			Vector2F logicalWindowSize;
			Vector2F windowScale;
			float logicalDpi = 0.0f;
			DisplayOrientations displayOrientation = DisplayOrientations::None;

			std::list<std::shared_ptr<Rendering::IRenderable>> renderables;

			void CreateD3DDevice();
			void CreateD2DDevice();
			void CreateDWriteFactory();
			void CreateSwapChain();
			void SetRenderTarget();

			void OnEvent(Event & event);

			// Converts between Windows display orientation and DXGI rotation.
			DXGI_MODE_ROTATION RenderSystem::ComputeDisplayRotation(DisplayOrientations displayOrientation, DisplayOrientations nativeOrientation);

			void OnAppWindowChanged(AppWindowChangedEvent appWindowChangedEvent);
			void OnAppSuspending();
			void OnAppWindowSizeChanged(AppWindowSizeChangedEvent appWindowSizeChangedEvent);
			void OnDisplayDpiChanged(DisplayDpiChangedEvent displayDpiChangedEvent);
			void OnDisplayOrientationChanged(DisplayOrientationChangedEvent displayOrientationChangedEvent);
			void OnDisplayContentsInvalidated();
			void OnEntityInitialized(Entity entity);
			void OnEntityRemoved(Entity entity);

			void OnDeviceLost();
			void CreateWindowSizeDependentResources();

			void DrawSprite(std::shared_ptr<UI::Sprite> sprite);
			void DrawLabel(std::shared_ptr<UI::Label> label);

			static bool SortByDepth(const std::shared_ptr<Rendering::IRenderable>& first, const std::shared_ptr<Rendering::IRenderable>& second);
		};
	}
}