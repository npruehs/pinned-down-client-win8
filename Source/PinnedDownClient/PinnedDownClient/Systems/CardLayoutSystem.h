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
#include "Events\DisconnectedFromServerEvent.h"
#include "Events\EntityIdMappingCreatedEvent.h"
#include "Events\EntityHoveredEvent.h"
#include "Events\EntityTappedEvent.h"
#include "Events\EntityUnhoveredEvent.h"
#include "Events\FightResolvedEvent.h"
#include "Events\MatchEndedEvent.h"
#include "Events\PowerChangedEvent.h"
#include "Events\RenderTargetChangedEvent.h"
#include "Events\ShipDamagedEvent.h"
#include "Events\StructureChangedEvent.h"

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

			const float cardOffset = 0.125f;
			const float firstAssignedCardPositionX = 0.03125f;
			const float assignedCardOffsetY = 0.28f;
			const float damageCardOffsetY = 0.025f;
			const float powerLabelOffsetX = 0.025f;
			const float structureLabelOffsetX = 0.025f;
			const float abilityLabelOffsetX = 0.0125f;
			const float abilityLabelOffsetY = 0.05f;
			const float playerHandPositionY = 0.15f;
			const float playerCardPositionY = 0.222f;
			const float enemyCardPositionY = -0.222f;

			void LoadResources();

			void OnEvent(Event & event);
			
			void Reset();

			EVENT_HANDLER_DECLARATION(CardAssignedEvent);
			EVENT_HANDLER_DECLARATION(CardCreatedEvent);
			EVENT_HANDLER_DECLARATION(CardStateChangedEvent);
			EVENT_HANDLER_DECLARATION(CardRemovedEvent);
			EVENT_HANDLER_DECLARATION(CardUnassignedEvent);
			EVENT_HANDLER_DECLARATION(EntityIdMappingCreatedEvent);
			EVENT_HANDLER_DECLARATION(EntityHoveredEvent);
			EVENT_HANDLER_DECLARATION(EntityTappedEvent);
			EVENT_HANDLER_DECLARATION(EntityUnhoveredEvent);
			EVENT_HANDLER_DECLARATION(FightResolvedEvent);
			EVENT_HANDLER_DECLARATION(MatchEndedEvent);
			EVENT_HANDLER_DECLARATION(PowerChangedEvent);
			EVENT_HANDLER_DECLARATION(RenderTargetChangedEvent);
			EVENT_HANDLER_DECLARATION(ShipDamagedEvent);
			EVENT_HANDLER_DECLARATION(StructureChangedEvent);
			EVENT_HANDLER_DECLARATION(DisconnectedFromServerEvent);

			std::shared_ptr<Card> CreateCard(Entity cardEntity);
			void LayoutCards();
			void RemoveCardEntity(std::shared_ptr<Card> card);
			
			Entity CardBackgroundToEntityId(Entity backgroundSprite);
			std::shared_ptr<Card> ServerEntityToCard(Entity serverEntity);
		};
	}
}
