#pragma once

#include "IEventListener.h"
#include "GameSystem.h"

#include "Data\TurnPhase.h"

#include "Events\CardTappedEvent.h"
#include "Events\TurnPhaseChangedEvent.h"
#include "Events\EntityIdMappingCreatedEvent.h"

#include "Util\EntityIdMapping.h"

using namespace PinnedDownNet::Data;
using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Events;

namespace PinnedDownClient
{
	namespace Systems
	{
		class CardPlayingSystem : public GameSystem, public IEventListener
		{
		public:
			CardPlayingSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::shared_ptr<EntityIdMapping> entityIdMapping;
			TurnPhase turnPhase;
			Entity selectedCard;

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(CardTappedEvent);
			EVENT_HANDLER_DECLARATION(EntityIdMappingCreatedEvent);
			EVENT_HANDLER_DECLARATION(TurnPhaseChangedEvent);

			void SelectCard(Entity card);
			void DeselectCard();
		};
	}
}
