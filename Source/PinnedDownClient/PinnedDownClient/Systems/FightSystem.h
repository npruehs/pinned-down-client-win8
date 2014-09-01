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
		class FightSystem : public GameSystem, public IEventListener
		{
		public:
			FightSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::shared_ptr<EntityIdMapping> entityIdMapping;
			TurnPhase turnPhase;

			void OnEvent(Event & event);

			void OnCardTapped(CardTappedEvent& cardTappedEvent);
			void OnEntityIdMappingCreated(EntityIdMappingCreatedEvent& entityIdMappingCreatedEvent);
			void OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent);
		};
	}
}
