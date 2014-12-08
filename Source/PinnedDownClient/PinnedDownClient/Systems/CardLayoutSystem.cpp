#include "pch.h"

#include <algorithm>

#include "Game.h"
#include "Event.h"
#include "EntityManager.h"

#include "Components\AffiliationComponent.h"
#include "Components\CardComponent.h"
#include "Components\CardStateComponent.h"
#include "Components\CardUIComponent.h"
#include "Components\FlagshipComponent.h"
#include "Components\OwnerComponent.h"
#include "Components\PowerComponent.h"
#include "Components\StructureComponent.h"
#include "Components\ThreatComponent.h"

#include "Events\CardTappedEvent.h"
#include "Events\DisconnectedFromServerEvent.h"
#include "Events\LocalizedTextChangedEvent.h"

#include "Resources\PinnedDownResourceManager.h"

#include "Systems\CardLayoutSystem.h"

#include "Util\StringUtils.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Resources;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Systems::CardLayout;
using namespace PinnedDownClient::Util;



CardLayoutSystem::CardLayoutSystem()
{
}

void CardLayoutSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	this->uiFactory = std::make_shared<UIFactory>(game);

	this->game->eventManager->AddListener(this, CardAssignedEvent::CardAssignedEventType);
	this->game->eventManager->AddListener(this, CardCreatedEvent::CardCreatedEventType);
	this->game->eventManager->AddListener(this, CardStateChangedEvent::CardStateChangedEventType);
	this->game->eventManager->AddListener(this, CardRemovedEvent::CardRemovedEventType);
	this->game->eventManager->AddListener(this, CardUnassignedEvent::CardUnassignedEventType);
	this->game->eventManager->AddListener(this, DisconnectedFromServerEvent::DisconnectedFromServerEventType);
	this->game->eventManager->AddListener(this, EntityHoveredEvent::EntityHoveredEventType);
	this->game->eventManager->AddListener(this, EntityTappedEvent::EntityTappedEventType);
	this->game->eventManager->AddListener(this, EntityUnhoveredEvent::EntityUnhoveredEventType);
	this->game->eventManager->AddListener(this, EntityIdMappingCreatedEvent::EntityIdMappingCreatedEventType);
	this->game->eventManager->AddListener(this, FightResolvedEvent::FightResolvedEventType);
	this->game->eventManager->AddListener(this, PowerChangedEvent::PowerChangedEventType);
	this->game->eventManager->AddListener(this, RenderTargetChangedEvent::RenderTargetChangedEventType);
	this->game->eventManager->AddListener(this, ShipDamagedEvent::ShipDamagedEventType);
}

void CardLayoutSystem::LoadResources()
{
	auto resourceManager = static_cast<PinnedDownResourceManager*>(this->game->resourceManager.get());

	resourceManager->LoadBitmapFromFile(
		d2dContext.Get(),
		L"Assets/BlueWingStarship.png"
		);
}

void CardLayoutSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == CardAssignedEvent::CardAssignedEventType)
	{
		CardAssignedEvent& cardAssignedEvent = static_cast<CardAssignedEvent&>(newEvent);
		this->OnCardAssigned(cardAssignedEvent);
	}
    else if (newEvent.GetEventType() == CardCreatedEvent::CardCreatedEventType)
	{
		CardCreatedEvent& cardCreatedEvent = static_cast<CardCreatedEvent&>(newEvent);
		this->OnCardCreated(cardCreatedEvent);
	}
	else if (newEvent.GetEventType() == CardRemovedEvent::CardRemovedEventType)
	{
		CardRemovedEvent& cardRemovedEvent = static_cast<CardRemovedEvent&>(newEvent);
		this->OnCardRemoved(cardRemovedEvent);
	}
	else if (newEvent.GetEventType() == CardStateChangedEvent::CardStateChangedEventType)
	{
		CardStateChangedEvent& cardStateChangedEvent = static_cast<CardStateChangedEvent&>(newEvent);
		this->OnCardStateChanged(cardStateChangedEvent);
	}
	else if (newEvent.GetEventType() == CardUnassignedEvent::CardUnassignedEventType)
	{
		CardUnassignedEvent& cardUnassignedEvent = static_cast<CardUnassignedEvent&>(newEvent);
		this->OnCardUnassigned(cardUnassignedEvent);
	}
	else if (newEvent.GetEventType() == DisconnectedFromServerEvent::DisconnectedFromServerEventType)
	{
		this->OnDisconnectedFromServer();
	}
	else if (newEvent.GetEventType() == EntityIdMappingCreatedEvent::EntityIdMappingCreatedEventType)
	{
		EntityIdMappingCreatedEvent& entityIdMappingCreatedEvent = static_cast<EntityIdMappingCreatedEvent&>(newEvent);
		this->OnEntityIdMappingCreated(entityIdMappingCreatedEvent);
	}
	else if (newEvent.GetEventType() == EntityHoveredEvent::EntityHoveredEventType)
	{
		EntityHoveredEvent& entityHoveredEvent = static_cast<EntityHoveredEvent&>(newEvent);
		this->OnEntityHovered(entityHoveredEvent);
	}
	else if(newEvent.GetEventType() == EntityTappedEvent::EntityTappedEventType)
	{
		EntityTappedEvent& entityTappedEvent = static_cast<EntityTappedEvent&>(newEvent);
		this->OnEntityTapped(entityTappedEvent);
	}
	else if (newEvent.GetEventType() == EntityUnhoveredEvent::EntityUnhoveredEventType)
	{
		EntityUnhoveredEvent& entityUnhoveredEvent = static_cast<EntityUnhoveredEvent&>(newEvent);
		this->OnEntityUnhovered(entityUnhoveredEvent);
	}
	else if (newEvent.GetEventType() == FightResolvedEvent::FightResolvedEventType)
	{
		FightResolvedEvent& fightResolvedEvent = static_cast<FightResolvedEvent&>(newEvent);
		this->OnFightResolved(fightResolvedEvent);
	}
	else if (newEvent.GetEventType() == PowerChangedEvent::PowerChangedEventType)
	{
		PowerChangedEvent& powerChangedEvent = static_cast<PowerChangedEvent&>(newEvent);
		this->OnPowerChanged(powerChangedEvent);
	}
	else if (newEvent.GetEventType() == RenderTargetChangedEvent::RenderTargetChangedEventType)
	{
		auto renderTargetChangedEvent = static_cast<RenderTargetChangedEvent&>(newEvent);
		this->OnRenderTargetChanged(renderTargetChangedEvent);
	}
	else if (newEvent.GetEventType() == ShipDamagedEvent::ShipDamagedEventType)
	{
		auto shipDamagedEvent = static_cast<ShipDamagedEvent&>(newEvent);
		this->OnShipDamaged(shipDamagedEvent);
	}
}

void CardLayoutSystem::OnCardAssigned(CardAssignedEvent& cardAssignedEvent)
{
	auto clientAssignedCard = this->entityIdMapping->ServerToClientId(cardAssignedEvent.assignedCard);
	auto clientTargetCard = this->entityIdMapping->ServerToClientId(cardAssignedEvent.targetCard);

	// Assign card.
	this->currentAssignments.insert(std::pair<Entity, Entity>(clientAssignedCard, clientTargetCard));

	// Update layout.
	this->LayoutCards();
}

void CardLayoutSystem::OnEntityIdMappingCreated(EntityIdMappingCreatedEvent& entityIdMappingCreatedEvent)
{
	this->entityIdMapping = entityIdMappingCreatedEvent.entityIdMapping;
}

void CardLayoutSystem::OnCardCreated(CardCreatedEvent& cardCreatedEvent)
{
	auto cardEntity = this->entityIdMapping->ServerToClientId(cardCreatedEvent.serverEntity);
	auto card = this->CreateCard(cardEntity);

	// Add to list.
	this->cards.push_back(card);

	// Update layout.
	this->LayoutCards();
}

void CardLayoutSystem::OnCardStateChanged(CardStateChangedEvent& cardStateChangedEvent)
{
	auto cardEntity = this->entityIdMapping->ServerToClientId(cardStateChangedEvent.serverEntity);
	auto cardStateComponent = this->game->entityManager->GetComponent<CardStateComponent>(cardEntity, CardStateComponent::CardStateComponentType);
	cardStateComponent->cardState = cardStateChangedEvent.newState;

	// Update layout.
	this->LayoutCards();
}

void CardLayoutSystem::OnCardRemoved(CardRemovedEvent& cardRemovedEvent)
{
	auto clientEntity = this->entityIdMapping->ServerToClientId(cardRemovedEvent.serverEntity);

	for (auto iterator = this->cards.begin(); iterator != this->cards.end(); iterator++)
	{
		auto card = *iterator;

		if (card->cardEntity == clientEntity)
		{
			this->RemoveCardEntity(card);
			this->cards.erase(iterator);
			return;
		}
	}
}

void CardLayoutSystem::OnCardUnassigned(CardUnassignedEvent& cardUnassignedEvent)
{
	auto clientAssignedCard = this->entityIdMapping->ServerToClientId(cardUnassignedEvent.assignedCard);

	// Remove assignment.
	auto assignment = this->currentAssignments.find(clientAssignedCard);

	if (assignment != this->currentAssignments.end())
	{
		this->currentAssignments.erase(assignment);
	}

	// Update layout.
	this->LayoutCards();
}

void CardLayoutSystem::OnDisconnectedFromServer()
{
	for (auto iterator = this->cards.begin(); iterator != this->cards.end(); iterator++)
	{
		auto card = *iterator;
		this->RemoveCardEntity(card);
	}

	this->cards.clear();
}

void CardLayoutSystem::OnEntityHovered(EntityHoveredEvent& entityHoveredEvent)
{
	// Find hovered card.
	auto cardEntity = this->CardBackgroundToEntityId(entityHoveredEvent.entity);

	if (cardEntity != INVALID_ENTITY_ID)
	{
		this->cardDetailView = this->CreateCard(cardEntity);
	}

	// Update layout.
	this->LayoutCards();
}

void CardLayoutSystem::OnEntityTapped(EntityTappedEvent& entityTappedEvent)
{
	// Find tapped card.
	auto cardEntity = this->CardBackgroundToEntityId(entityTappedEvent.entity);

	if (cardEntity != INVALID_ENTITY_ID)
	{
		// Notify listeners.
		auto cardTappedEvent = std::make_shared<CardTappedEvent>(cardEntity);
		this->game->eventManager->QueueEvent(cardTappedEvent);
	}
}

void CardLayoutSystem::OnEntityUnhovered(EntityUnhoveredEvent& entityUnhoveredEvent)
{
	// Find unhovered card.
	auto cardEntity = this->CardBackgroundToEntityId(entityUnhoveredEvent.entity);

	if (cardEntity != INVALID_ENTITY_ID)
	{
		if (this->cardDetailView != nullptr && this->cardDetailView->cardEntity == cardEntity)
		{
			// Remove card.
			this->RemoveCardEntity(this->cardDetailView);
			this->cardDetailView = nullptr;
		}
	}

	// Update layout.
	this->LayoutCards();
}

void CardLayoutSystem::OnFightResolved(FightResolvedEvent& fightResolvedEvent)
{
	auto clientAssignedCard = this->entityIdMapping->ServerToClientId(fightResolvedEvent.assignedCard);

	// Remove assignment
	auto assignment = this->currentAssignments.find(clientAssignedCard);

	if (assignment != this->currentAssignments.end())
	{
		this->currentAssignments.erase(assignment);
	}

	// Update layout.
	this->LayoutCards();
}

void CardLayoutSystem::OnPowerChanged(PowerChangedEvent& powerChangedEvent)
{
	auto card = this->ServerEntityToCard(powerChangedEvent.entity);

	if (card != nullptr)
	{
		this->uiFactory->SetText(card->powerValueLabel, std::to_wstring(powerChangedEvent.newPower));
	}
}

void CardLayoutSystem::OnRenderTargetChanged(RenderTargetChangedEvent& renderTargetChangedEvent)
{
	this->d2dContext = renderTargetChangedEvent.d2dContext;

	this->LoadResources();
}

void CardLayoutSystem::OnShipDamaged(ShipDamagedEvent& shipDamagedEvent)
{
	Entity damageCardEntity = this->entityIdMapping->ServerToClientId(shipDamagedEvent.damageCard);
	Entity shipCardEntity = this->entityIdMapping->ServerToClientId(shipDamagedEvent.damagedShip);

	std::shared_ptr<Card> damageCard;
	std::shared_ptr<Card> shipCard;

	// Find cards.
	for (auto iterator = this->cards.begin(); iterator != this->cards.end(); iterator++)
	{
		auto card = *iterator;

		if (card->cardEntity == damageCardEntity)
		{
			damageCard = card;
		}
		else if (card->cardEntity == shipCardEntity)
		{
			shipCard = card;
		}

		if (damageCard != nullptr && shipCard != nullptr)
		{
			break;
		}
	}

	// Get last attached damage card.
	auto damagedShipsIterator = this->damagedShips.find(shipCardEntity);

	if (damagedShipsIterator == this->damagedShips.end())
	{
		// Store damage layout data.
		auto damageLayoutData = DamageLayoutData();
		damageLayoutData.panelDepth = -10;

		// Anchor damage card to ship.
		this->uiFactory->SetAnchor(damageCard->backgroundSprite, VerticalAnchor(VerticalAnchorType::VerticalCenter, this->damageCardOffset), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0), shipCard->backgroundSprite);
		this->uiFactory->SetDepth(damageCard->panel, damageLayoutData.panelDepth);

		damageLayoutData.background = damageCard->backgroundSprite;

		this->damagedShips.insert(std::pair<Entity, DamageLayoutData>(shipCardEntity, damageLayoutData));
	}
	else
	{
		// Update damage layout data.
		auto damageLayoutData = &damagedShipsIterator->second;
		damageLayoutData->panelDepth--;

		// Anchor damage card to last damage card.
		this->uiFactory->SetAnchor(damageCard->backgroundSprite, VerticalAnchor(VerticalAnchorType::VerticalCenter, this->damageCardOffset), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0), damageLayoutData->background);
		this->uiFactory->SetDepth(damageCard->panel, damageLayoutData->panelDepth);

		damageLayoutData->background = damageCard->backgroundSprite;
	}
}

std::shared_ptr<Card> CardLayoutSystem::CreateCard(Entity cardEntity)
{
	auto card = std::make_shared<Card>();
	card->cardEntity = cardEntity;

	// Card panel.
	card->panel = this->uiFactory->CreatePanel();

	// Card background sprite.
	card->backgroundSprite = this->uiFactory->CreateSprite("Assets/BlueWingStarship.png");
	this->uiFactory->SetAnchor(card->backgroundSprite, VerticalAnchor(VerticalAnchorType::VerticalCenter, 200.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->SetPanel(card->backgroundSprite, card->panel);
	this->uiFactory->SetTappable(card->backgroundSprite);
	this->uiFactory->FinishUIWidget(card->backgroundSprite);

	// Name label.
	auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(card->cardEntity, CardComponent::CardComponentType);

	card->nameLabel = this->uiFactory->CreateLabel(L"Card_" + std::to_wstring(cardComponent->setIndex) + L"_" + std::to_wstring(cardComponent->cardIndex));
	this->uiFactory->SetAnchor(card->nameLabel, VerticalAnchor(VerticalAnchorType::Top, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), card->backgroundSprite);
	this->uiFactory->SetColor(card->nameLabel, D2D1::ColorF(D2D1::ColorF::Black));
	this->uiFactory->SetPanel(card->nameLabel, card->panel);
	this->uiFactory->FinishUIWidget(card->nameLabel);

	// Card type label.
	auto cardType = StringToWString(CardTypeToString(cardComponent->cardType));

	card->cardTypeLabel = this->uiFactory->CreateLabel(L"Card_Type_" + StringToWString(CardTypeToString(cardComponent->cardType)));
	this->uiFactory->SetAnchor(card->cardTypeLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), card->backgroundSprite);
	this->uiFactory->SetColor(card->cardTypeLabel, D2D1::ColorF(D2D1::ColorF::Black));
	this->uiFactory->SetPanel(card->cardTypeLabel, card->panel);
	this->uiFactory->FinishUIWidget(card->cardTypeLabel);

	// Threat label.
	auto threatComponent = this->game->entityManager->GetComponent<ThreatComponent>(card->cardEntity, ThreatComponent::ThreatComponentType);

	if (threatComponent != nullptr)
	{
		card->threatLabel = this->uiFactory->CreateLabel(L"[" + std::to_wstring(threatComponent->threat) + L"]");
		this->uiFactory->SetAnchor(card->threatLabel, VerticalAnchor(VerticalAnchorType::Top, 0.0f), HorizontalAnchor(HorizontalAnchorType::Left, 0.0f), card->backgroundSprite);
		this->uiFactory->SetColor(card->threatLabel, D2D1::ColorF(D2D1::ColorF::Black));
		this->uiFactory->SetPanel(card->threatLabel, card->panel);
		this->uiFactory->FinishUIWidget(card->threatLabel);
	}

	// Power label.
	auto powerComponent = this->game->entityManager->GetComponent<PowerComponent>(card->cardEntity, PowerComponent::PowerComponentType);

	if (powerComponent != nullptr)
	{
		card->powerLabel = this->uiFactory->CreateLabel(L"Card_Property_Power");
		this->uiFactory->SetAnchor(card->powerLabel, VerticalAnchor(VerticalAnchorType::Bottom, 0.0f), HorizontalAnchor(HorizontalAnchorType::Left, 0.0f), card->backgroundSprite);
		this->uiFactory->SetColor(card->powerLabel, D2D1::ColorF(D2D1::ColorF::Black));
		this->uiFactory->SetPanel(card->powerLabel, card->panel);
		this->uiFactory->FinishUIWidget(card->powerLabel);

		card->powerValueLabel = this->uiFactory->CreateLabel(std::to_wstring(powerComponent->power));
		this->uiFactory->SetAnchor(card->powerValueLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::Left, 40.0f), card->powerLabel);
		this->uiFactory->SetColor(card->powerValueLabel, D2D1::ColorF(D2D1::ColorF::Black));
		this->uiFactory->SetPanel(card->powerValueLabel, card->panel);
		this->uiFactory->FinishUIWidget(card->powerValueLabel);
	}

	// Structure label.
	auto structureComponent = this->game->entityManager->GetComponent<StructureComponent>(card->cardEntity, StructureComponent::StructureComponentType);

	if (structureComponent != nullptr)
	{
		card->structureLabel = this->uiFactory->CreateLabel(L"Card_Property_Structure");
		this->uiFactory->SetAnchor(card->structureLabel, VerticalAnchor(VerticalAnchorType::Bottom, 0.0f), HorizontalAnchor(HorizontalAnchorType::Right, -40.0f), card->backgroundSprite);
		this->uiFactory->SetColor(card->structureLabel, D2D1::ColorF(D2D1::ColorF::Black));
		this->uiFactory->SetPanel(card->structureLabel, card->panel);
		this->uiFactory->FinishUIWidget(card->structureLabel);

		card->structureValueLabel = this->uiFactory->CreateLabel(std::to_wstring(structureComponent->structure) + L"%");
		this->uiFactory->SetAnchor(card->structureValueLabel, VerticalAnchor(VerticalAnchorType::Bottom, 0.0f), HorizontalAnchor(HorizontalAnchorType::Right, 0.0f), card->backgroundSprite);
		this->uiFactory->SetColor(card->structureValueLabel, D2D1::ColorF(D2D1::ColorF::Black));
		this->uiFactory->SetPanel(card->structureValueLabel, card->panel);
		this->uiFactory->FinishUIWidget(card->structureValueLabel);
	}

	// Ability label.
	auto flagshipComponent = this->game->entityManager->GetComponent<FlagshipComponent>(card->cardEntity, FlagshipComponent::FlagshipComponentType);

	card->abilityLabel = this->uiFactory->CreateLabel(flagshipComponent != nullptr ? L"Card_Ability_Flagship" : L"");
	this->uiFactory->SetAnchor(card->abilityLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 30.0f), HorizontalAnchor(HorizontalAnchorType::Left, 20.0f), card->backgroundSprite);
	this->uiFactory->SetColor(card->abilityLabel, D2D1::ColorF(D2D1::ColorF::Black));
	this->uiFactory->SetPanel(card->abilityLabel, card->panel);
	this->uiFactory->FinishUIWidget(card->abilityLabel);

	auto cardUiComponent = std::make_shared<CardUIComponent>();
	cardUiComponent->background = card->backgroundSprite;
	this->game->entityManager->AddComponent(card->cardEntity, cardUiComponent);

	return card;
}

void CardLayoutSystem::LayoutCards()
{
	// Count cards.
	int assignedPlayerCards = 0;
	int unassignedPlayerCards = 0;
	int playerHandCards = 0;
	int enemyCards = 0;

	for (auto iterator = this->cards.begin(); iterator != this->cards.end(); iterator++)
	{
		auto card = *iterator;
		auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(card->cardEntity, CardComponent::CardComponentType);
		auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(card->cardEntity, OwnerComponent::OwnerComponentType);
		auto cardStateComponent = this->game->entityManager->GetComponent<CardStateComponent>(card->cardEntity, CardStateComponent::CardStateComponentType);

		if (ownerComponent->owner != INVALID_ENTITY_ID)
		{
			// Player card - check if assigned.
			auto assignment = this->currentAssignments.find(card->cardEntity);

			if (assignment != this->currentAssignments.end())
			{
				// Card is assigned to an enemy.
				assignedPlayerCards++;
			}
			else if (cardStateComponent->cardState == CardState::Hand)
			{
				// Card is in hand.
				playerHandCards++;
			}
			else
			{
				// Card is not assigned.
				unassignedPlayerCards++;
			}
		}
		else
		{
			if (cardComponent->cardType != CardType::Damage)
			{
				enemyCards++;
			}
		}
	}

	int assignedEnemyCards = assignedPlayerCards;
	int unassignedEnemyCards = enemyCards - assignedEnemyCards;

	// Layout cards.

	// Compute offset between assigned cards to the left and unassigned cards to the right.
	float unassignedCardOffset = assignedPlayerCards * (cardWidth + cardOffset);

	// Center unassigned player cards to the right.
	float unassignedPlayerCardPositionX = -(unassignedPlayerCards - 1) * (cardWidth + cardOffset) / 2 + unassignedCardOffset;

	// Center player hand cards.
	float playerHandCardPositionX = -(playerHandCards - 1) * (cardWidth + cardOffset) / 2;

	// Align assigned enemy cards left. Assigned player cards will be anchored to their respective enemies.
	float assignedEnemyCardPositionX = firstAssignedCardPositionX;

	// Center unassigned player cards to the right.
	float unassignedEnemyCardPositionX = -(unassignedEnemyCards - 1) * (cardWidth + cardOffset) / 2 + unassignedCardOffset;

	for (auto iterator = this->cards.begin(); iterator != this->cards.end(); ++iterator)
	{
		auto card = *iterator;
		auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(card->cardEntity, CardComponent::CardComponentType);
		auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(card->cardEntity, OwnerComponent::OwnerComponentType);
		auto cardStateComponent = this->game->entityManager->GetComponent<CardStateComponent>(card->cardEntity, CardStateComponent::CardStateComponentType);

		if (ownerComponent->owner != INVALID_ENTITY_ID)
		{
			// Player card - check if assigned.
			auto assignment = this->currentAssignments.find(card->cardEntity);

			if (assignment != this->currentAssignments.end())
			{
				// Card is assigned to an enemy.
				auto targetCard = assignment->second;
				auto targetCardUiComponent = this->game->entityManager->GetComponent<CardUIComponent>(targetCard, CardUIComponent::CardUIComponentType);

				this->uiFactory->SetAnchor(card->backgroundSprite, VerticalAnchor(VerticalAnchorType::VerticalCenter, this->assignedCardOffsetY), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), targetCardUiComponent->background);
			}
			else if (cardStateComponent->cardState == CardState::Hand)
			{
				// Card is in hand.
				this->uiFactory->SetAnchor(card->backgroundSprite, VerticalAnchor(VerticalAnchorType::Bottom, this->playerHandPositionY), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, playerHandCardPositionX), 0);
				playerHandCardPositionX += cardWidth + cardOffset;
			}
			else
			{
				// Card is not assigned.
				this->uiFactory->SetAnchor(card->backgroundSprite, VerticalAnchor(VerticalAnchorType::VerticalCenter, this->playerCardPositionY), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, unassignedPlayerCardPositionX), 0);
				unassignedPlayerCardPositionX += cardWidth + cardOffset;
			}
		}
		else
		{
			// Not a player card, check card type.
			if (cardComponent->cardType != CardType::Damage)
			{
				// Enemy card - check if assigned.
				bool assigned = false;

				for (auto assignmentIt = this->currentAssignments.begin(); assignmentIt != this->currentAssignments.end(); ++assignmentIt)
				{
					auto assignedEnemy = assignmentIt->second;
					if (assignedEnemy == card->cardEntity)
					{
						assigned = true;
						break;
					}
				}

				if (assigned)
				{
					this->uiFactory->SetAnchor(card->backgroundSprite, VerticalAnchor(VerticalAnchorType::VerticalCenter, this->enemyCardPositionY), HorizontalAnchor(HorizontalAnchorType::Left, assignedEnemyCardPositionX), 0);
					assignedEnemyCardPositionX += cardWidth + cardOffset;
				}
				else
				{
					this->uiFactory->SetAnchor(card->backgroundSprite, VerticalAnchor(VerticalAnchorType::VerticalCenter, this->enemyCardPositionY), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, unassignedEnemyCardPositionX), 0);
					unassignedEnemyCardPositionX += cardWidth + cardOffset;
				}
			}
		}
	}

	// Layout card preview.
	if (this->cardDetailView != nullptr)
	{
		this->uiFactory->SetAnchor(this->cardDetailView->backgroundSprite, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::Left, 0.0f), 0);
	}	
}

void CardLayoutSystem::RemoveCardEntity(std::shared_ptr<Card> card)
{
	this->game->entityManager->RemoveEntity(card->panel);
	this->game->entityManager->RemoveEntity(card->backgroundSprite);
	this->game->entityManager->RemoveEntity(card->cardTypeLabel);
	this->game->entityManager->RemoveEntity(card->nameLabel);
	this->game->entityManager->RemoveEntity(card->powerLabel);
	this->game->entityManager->RemoveEntity(card->powerValueLabel);
	this->game->entityManager->RemoveEntity(card->threatLabel);
	this->game->entityManager->RemoveEntity(card->abilityLabel);
	this->game->entityManager->RemoveEntity(card->structureLabel);
}

Entity CardLayoutSystem::CardBackgroundToEntityId(Entity backgroundSprite)
{
	for (auto iterator = this->cards.begin(); iterator != this->cards.end(); iterator++)
	{
		auto card = *iterator;

		if (card->backgroundSprite == backgroundSprite)
		{
			return card->cardEntity;
		}
	}

	return INVALID_ENTITY_ID;
}

std::shared_ptr<Card> CardLayoutSystem::ServerEntityToCard(Entity serverEntity)
{
	auto clientEntity = this->entityIdMapping->ServerToClientId(serverEntity);

	for (auto iterator = this->cards.begin(); iterator != this->cards.end(); iterator++)
	{
		auto card = *iterator;

		if (card->cardEntity == clientEntity)
		{
			return card;
		}
	}

	return nullptr;
}