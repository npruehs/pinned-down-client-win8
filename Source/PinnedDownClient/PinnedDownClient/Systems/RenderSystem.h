#pragma once

#include "../Core/ISystem.h"
#include "../Helpers/DirectXHelper.h"

using namespace Microsoft::WRL;
using namespace PinnedDownClient::Core;

namespace PinnedDownClient
{
	namespace Systems
	{
		class RenderSystem : public Core::ISystem
		{
		public:
			RenderSystem(Windows::UI::Core::CoreWindow^ window);

			void InitSystem();
			void Update(DX::StepTimer const& timer);

		private:
			Platform::Agile<Windows::UI::Core::CoreWindow>	window;

			// Direct3D hardware device for the default adapter.
			ComPtr<ID3D11Device2> d3dDevice;

			// Feature level of the current Direct3D device.
			D3D_FEATURE_LEVEL d3dFeatureLevel;

			// Underlying DirectX Graphics Infrastructure device of the Direct3D device. Used for retrieving the Direct2D device.
			ComPtr<IDXGIDevice3> dxgiDevice;

			// Swap chain presenting our results.
			ComPtr<IDXGISwapChain1> dxgiSwapChain;

			// Set of state and command buffers used to render to a target. Call methods on this context to set pipeline state and generate rendering commands.
			ComPtr<ID2D1DeviceContext> d2dContext;

			void CreateD3DDevice();
			void CreateD2DDevice();
			void CreateSwapChain();
			void SetRenderTarget();
		};
	}
}