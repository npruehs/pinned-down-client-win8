#pragma once

#include "PinnedDownGame.h"
#include "Core\GameSystem.h"
#include "Core\IEventListener.h"

#include "Events\EntityInitializedEvent.h"
#include "Events\EntityRemovedEvent.h"
#include "Events\PointerReleasedEvent.h"

#include "Systems\UI\Button.h"

using namespace PinnedDownClient::Core;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems::UI;

namespace PinnedDownClient
{
	namespace Systems
	{
		class UIInteractionSystem : public Core::GameSystem, public Core::IEventListener
		{
		public:
			UIInteractionSystem();

			void InitSystem(std::shared_ptr<GameInfrastructure> game);

		private:
			void OnEvent(Event & event);

			std::list<Button> buttons;

			void OnEntityInitialized(EntityInitializedEvent entityInitializedEvent);
			void OnEntityRemoved(EntityRemovedEvent entityRemovedEvent);
			void OnPointerReleased(PointerReleasedEvent pointerReleasedEvent);
		};
	}
}