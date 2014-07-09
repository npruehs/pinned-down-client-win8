#pragma once

#include "Core\GameSystem.h"
#include "Core\IEventListener.h"
#include "Events\RenderTargetChangedEvent.h"

using namespace Microsoft::WRL;
using namespace PinnedDownClient::Core;
using namespace PinnedDownClient::Events;

namespace PinnedDownClient
{
	namespace Systems
	{
		class RenderTextSystem : public Core::GameSystem, public Core::IEventListener
		{
		public:
			RenderTextSystem();

			void InitSystem(std::shared_ptr<Core::EventManager> eventManager);
			void Render();

			void OnEvent(Event & event);

		private:
			// Set of state and command buffers used to render to a target. Call methods on this context to set pipeline state and generate rendering commands.
			ComPtr<ID2D1DeviceContext> d2dContext;

			void OnRenderTargetChanged(RenderTargetChangedEvent renderTargetChangedEvent);
		};
	}
}