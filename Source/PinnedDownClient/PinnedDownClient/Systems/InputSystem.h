#pragma once

#include "PinnedDownGame.h"
#include "Core\GameSystem.h"
#include "Core\IEventListener.h"
#include "Events\AppWindowChangedEvent.h"
#include "Events\AppWindowSizeChangedEvent.h"
#include "Events\DisplayDpiChangedEvent.h"
#include "Events\DisplayOrientationChangedEvent.h"

using namespace Microsoft::WRL;
using namespace Windows::Graphics::Display;

using namespace PinnedDownClient::Core;
using namespace PinnedDownClient::Events;

namespace PinnedDownClient
{
	namespace Systems
	{
		class InputSystem : public Core::GameSystem, public Core::IEventListener
		{
		public:
			InputSystem();

			void InitSystem(PinnedDownClient::PinnedDownGame* game);

			void OnEvent(Event & event);

		private:
			void OnAppWindowChanged(AppWindowChangedEvent appWindowChangedEvent);
			void OnAppWindowSizeChanged(AppWindowSizeChangedEvent appWindowSizeChangedEvent);
			void OnDisplayDpiChanged(DisplayDpiChangedEvent displayDpiChangedEvent);
			void OnDisplayOrientationChanged(DisplayOrientationChangedEvent displayOrientationChangedEvent);
		};
	}
}