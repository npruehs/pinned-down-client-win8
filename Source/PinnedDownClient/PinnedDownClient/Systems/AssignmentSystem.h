#pragma once

#include "IEventListener.h"
#include "GameSystem.h"

#include "Events\CardAssignedEvent.h"
#include "Events\CardTappedEvent.h"
#include "Events\EntityIdMappingCreatedEvent.h"

#include "Util\EntityIdMapping.h"

using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Events;

namespace PinnedDownClient
{
	namespace Systems
	{
		class AssignmentSystem : public GameSystem, public IEventListener
		{
		public:
			AssignmentSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			Entity selectedCard;
			std::shared_ptr<EntityIdMapping> entityIdMapping;

			void OnEvent(Event & event);

			void OnCardAssigned(CardAssignedEvent& cardAssignedEvent);
			void OnCardTapped(CardTappedEvent& cardTappedEvent);
			void OnEntityIdMappingCreated(EntityIdMappingCreatedEvent& entityIdMappingCreatedEvent);

			void SelectCard(Entity card);
			void DeselectCard();
		};
	}
}
