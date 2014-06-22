#pragma once

#include "../Core/ISystem.h"
#include "../Helpers/DirectXHelper.h"
#include "Core\IEventListener.h"
#include "Events\AppWindowChangedEvent.h"
#include "Events\AppSuspendingEvent.h"
#include "Events\AppWindowSizeChangedEvent.h"
#include "Events\DisplayDpiChangedEvent.h"
#include "Events\DisplayOrientationChangedEvent.h"
#include "Events\DisplayContentsInvalidatedEvent.h"

using namespace Microsoft::WRL;
using namespace Windows::Graphics::Display;

using namespace PinnedDownClient::Core;
using namespace PinnedDownClient::Events;

namespace PinnedDownClient
{
	namespace Systems
	{
		class RenderSystem : public Core::ISystem, public Core::IEventListener
		{
		public:
			RenderSystem();

			void InitSystem(std::shared_ptr<Core::EventManager> eventManager);
			void Update(DX::StepTimer const& timer);

			void OnEvent(Event & event);

		private:
			Platform::Agile<Windows::UI::Core::CoreWindow>	window;

			// Direct3D hardware device for the default adapter.
			ComPtr<ID3D11Device2> d3dDevice;

			// Set of state and command buffers used to render to a target. Call methods on this context to set pipeline state and generate rendering commands.
			ComPtr<ID3D11DeviceContext> d3dContext;

			// Feature level of the current Direct3D device.
			D3D_FEATURE_LEVEL d3dFeatureLevel;

			// Underlying DirectX Graphics Infrastructure device of the Direct3D device. Used for retrieving the Direct2D device.
			ComPtr<IDXGIDevice3> dxgiDevice;

			// Swap chain presenting our results.
			ComPtr<IDXGISwapChain1> dxgiSwapChain;

			// Use double buffering to enable flip.
			float swapChainBufferCount = 2;

			// Set of state and command buffers used to render to a target. Call methods on this context to set pipeline state and generate rendering commands.
			ComPtr<ID2D1DeviceContext> d2dContext;

			// Anything rendered to this bitmap is rendered to the surface of the swap chain.
			ComPtr<ID2D1Bitmap1> d2dTargetBitmap;

			float logicalWindowWidth;
			float logicalWindowHeight;
			float logicalDpi;
			DisplayOrientations displayOrientation;

			void CreateD3DDevice();
			void CreateD2DDevice();
			void CreateSwapChain();
			void SetRenderTarget();

			// Converts between Windows display orientation and DXGI rotation.
			DXGI_MODE_ROTATION RenderSystem::ComputeDisplayRotation(DisplayOrientations displayOrientation, DisplayOrientations nativeOrientation);

			void OnAppWindowChanged(AppWindowChangedEvent appWindowChangedEvent);
			void OnAppSuspending();
			void OnAppWindowSizeChanged(AppWindowSizeChangedEvent appWindowSizeChangedEvent);
			void OnDisplayDpiChanged(DisplayDpiChangedEvent displayDpiChangedEvent);
			void OnDisplayOrientationChanged(DisplayOrientationChangedEvent displayOrientationChangedEvent);
			void OnDisplayContentsInvalidated();

			void OnDeviceLost();
			void CreateWindowSizeDependentResources();
		};
	}
}