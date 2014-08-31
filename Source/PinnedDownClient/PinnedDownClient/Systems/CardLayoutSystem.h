#pragma once

#include <map>

#include "IEventListener.h"
#include "GameSystem.h"

#include "Events\EntityIdMappingCreatedEvent.h"
#include "Events\EntityTappedEvent.h"
#include "Events\CardAssignedEvent.h"
#include "Events\CardCreatedEvent.h"
#include "Events\CardRemovedEvent.h"
#include "Events\RenderTargetChangedEvent.h"

#include "Systems\UI\UIFactory.h"
#include "Systems\UI\Card.h"

#include "Util\EntityIdMapping.h"


using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems::UI;
using namespace PinnedDownClient::Util;


namespace PinnedDownClient
{
	namespace Systems
	{
		class CardLayoutSystem : public GameSystem, public IEventListener
		{
		public:
			CardLayoutSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::map<Entity, Entity> currentAssignments;

			std::shared_ptr<EntityIdMapping> entityIdMapping;
			std::shared_ptr<UIFactory> uiFactory;
			ComPtr<ID2D1DeviceContext> d2dContext;
			std::list<std::shared_ptr<Card>> cards;

			float playerCardPositionY = 200;
			float enemyCardPositionY = -200;
			float cardWidth = 200;
			float cardOffset = 50;
			float assignedCardOffset = 300;

			void LoadResources();

			void OnEvent(Event & event);

			void OnCardAssigned(CardAssignedEvent& cardAssignedEvent);
			void OnCardCreated(CardCreatedEvent& cardCreatedEvent);
			void OnCardRemoved(CardRemovedEvent& cardRemovedEvent);
			void OnEntityIdMappingCreated(EntityIdMappingCreatedEvent& entityIdMappingCreatedEvent);
			void OnEntityTapped(EntityTappedEvent& entityTappedEvent);
			void OnRenderTargetChanged(RenderTargetChangedEvent& renderTargetChangedEvent);

			void LayoutCards();
		};
	}
}
