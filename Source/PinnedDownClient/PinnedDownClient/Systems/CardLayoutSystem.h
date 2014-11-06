#pragma once

#include <map>

#include "IEventListener.h"
#include "GameSystem.h"

#include "Systems\CardLayout\DamageLayoutData.h"

#include "Events\CardAssignedEvent.h"
#include "Events\CardCreatedEvent.h"
#include "Events\CardStateChangedEvent.h"
#include "Events\CardRemovedEvent.h"
#include "Events\CardUnassignedEvent.h"
#include "Events\EntityIdMappingCreatedEvent.h"
#include "Events\EntityHoveredEvent.h"
#include "Events\EntityTappedEvent.h"
#include "Events\EntityUnhoveredEvent.h"
#include "Events\FightResolvedEvent.h"
#include "Events\PowerChangedEvent.h"
#include "Events\RenderTargetChangedEvent.h"
#include "Events\ShipDamagedEvent.h"

#include "Systems\UI\UIFactory.h"
#include "Systems\UI\Card.h"

#include "Util\EntityIdMapping.h"


using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems::UI;
using namespace PinnedDownClient::Systems::CardLayout;
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
			std::map<Entity, DamageLayoutData> damagedShips;
			std::list<Entity> handCards;

			std::shared_ptr<EntityIdMapping> entityIdMapping;
			std::shared_ptr<UIFactory> uiFactory;
			ComPtr<ID2D1DeviceContext> d2dContext;
			std::list<std::shared_ptr<Card>> cards;

			std::shared_ptr<Card> cardDetailView;

			const float cardWidth = 200;
			const float cardOffset = 50;
			const float assignedCardOffset = 300;
			const float damageCardOffset = 25;

			float playerHandPositionY = 200;
			float playerCardPositionY = 200;
			float enemyCardPositionY = -200;

			void LoadResources();

			void OnEvent(Event & event);

			void OnCardAssigned(CardAssignedEvent& cardAssignedEvent);
			void OnCardCreated(CardCreatedEvent& cardCreatedEvent);
			void OnCardStateChanged(CardStateChangedEvent& cardStateChangedEvent);
			void OnCardRemoved(CardRemovedEvent& cardRemovedEvent);
			void OnCardUnassigned(CardUnassignedEvent& cardUnassignedEvent);
			void OnEntityIdMappingCreated(EntityIdMappingCreatedEvent& entityIdMappingCreatedEvent);
			void OnEntityHovered(EntityHoveredEvent& entityHoveredEvent);
			void OnEntityTapped(EntityTappedEvent& entityTappedEvent);
			void OnEntityUnhovered(EntityUnhoveredEvent& entityUnhoveredEvent);
			void OnFightResolved(FightResolvedEvent& fightResolvedEvent);
			void OnPowerChanged(PowerChangedEvent& powerChangedEvent);
			void OnRenderTargetChanged(RenderTargetChangedEvent& renderTargetChangedEvent);
			void OnShipDamaged(ShipDamagedEvent& shipDamagedEvent);
			void OnDisconnectedFromServer();

			std::shared_ptr<Card> CreateCard(Entity cardEntity);
			void LayoutCards();
			void RemoveCardEntity(std::shared_ptr<Card> card);
			
			Entity CardBackgroundToEntityId(Entity backgroundSprite);
			std::shared_ptr<Card> ServerEntityToCard(Entity serverEntity);
		};
	}
}
