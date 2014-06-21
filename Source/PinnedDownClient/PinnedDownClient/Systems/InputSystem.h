#pragma once

#include "../Core/ISystem.h"
#include "Core\IEventListener.h"
#include "Events\AppWindowChangedEvent.h"
#include "Events\AppWindowSizeChangedEvent.h"

using namespace Microsoft::WRL;
using namespace PinnedDownClient::Core;
using namespace Windows::Graphics::Display;

namespace PinnedDownClient
{
	namespace Systems
	{
		class InputSystem : public Core::ISystem, public Core::IEventListener
		{
		public:
			InputSystem();

			void InitSystem(std::shared_ptr<Core::EventManager> eventManager);
			void Update(DX::StepTimer const& timer);

			void OnEvent(Event & event);

		private:
			void OnAppWindowChanged(Events::AppWindowChangedEvent appWindowChangedEvent);
			void OnAppWindowSizeChanged(Events::AppWindowSizeChangedEvent appWindowSizeChanged);
		};
	}
}