#pragma once

#include "PinnedDownGame.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Events\PointerMovedEvent.h"
#include "Events\PointerPressedEvent.h"
#include "Events\PointerReleasedEvent.h"


using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;


namespace PinnedDownClient
{
	namespace Systems
	{
		class InputSystem : public GameSystem, public IEventListener
		{
		public:
			InputSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			bool fingerDown;
			Vector2F lastPosition;

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(PointerMovedEvent);
			EVENT_HANDLER_DECLARATION(PointerPressedEvent);
			EVENT_HANDLER_DECLARATION(PointerReleasedEvent);
		};
	}
}