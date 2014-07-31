#pragma once

#include "PinnedDownGame.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Events\EntityInitializedEvent.h"
#include "EntityRemovedEvent.h"
#include "Events\PointerReleasedEvent.h"

#include "Systems\UI\Button.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems::UI;

namespace PinnedDownClient
{
	namespace Systems
	{
		class UIInteractionSystem : public GameSystem, public IEventListener
		{
		public:
			UIInteractionSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			void OnEvent(Event & event);

			std::list<Button> buttons;

			void OnEntityInitialized(EntityInitializedEvent entityInitializedEvent);
			void OnEntityRemoved(EntityRemovedEvent entityRemovedEvent);
			void OnPointerReleased(PointerReleasedEvent pointerReleasedEvent);
		};
	}
}