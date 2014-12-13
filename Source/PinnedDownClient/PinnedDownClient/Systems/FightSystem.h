#pragma once

#include "IEventListener.h"
#include "GameSystem.h"

#include "Data\TurnPhase.h"

#include "Events\CardTappedEvent.h"
#include "Events\EntityIdMappingCreatedEvent.h"
#include "Events\TurnPhaseChangedEvent.h"
#include "Events\UIModeChangedEvent.h"

#include "Systems\UI\UIMode.h"

#include "Util\EntityIdMapping.h"

using namespace PinnedDownNet::Data;
using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Events;

namespace PinnedDownClient
{
	namespace Systems
	{
		class FightSystem : public GameSystem, public IEventListener
		{
		public:
			FightSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::shared_ptr<EntityIdMapping> entityIdMapping;
			TurnPhase turnPhase;
			UI::UIMode currentUIMode;

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(CardTappedEvent);
			EVENT_HANDLER_DECLARATION(EntityIdMappingCreatedEvent);
			EVENT_HANDLER_DECLARATION(TurnPhaseChangedEvent);
			EVENT_HANDLER_DECLARATION(UIModeChangedEvent);
		};
	}
}
