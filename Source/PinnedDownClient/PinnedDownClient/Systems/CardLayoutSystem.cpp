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
#include "Components\PositionComponent.h"
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
	this->game->eventManager->AddListener(this, CardCameraCreatedEvent::CardCameraCreatedEventType);
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
	this->game->eventManager->AddListener(this, MatchEndedEvent::MatchEndedEventType);
	this->game->eventManager->AddListener(this, PlayerAddedEvent::PlayerAddedEventType);
	this->game->eventManager->AddListener(this, PositionChangedEvent::PositionChangedEventType);
	this->game->eventManager->AddListener(this, PowerChangedEvent::PowerChangedEventType);
	this->game->eventManager->AddListener(this, RenderTargetChangedEvent::RenderTargetChangedEventType);
	this->game->eventManager->AddListener(this, ShipDamagedEvent::ShipDamagedEventType);
	this->game->eventManager->AddListener(this, StructureChangedEvent::StructureChangedEventType);
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
	CALL_EVENT_HANDLER(CardAssignedEvent);
	CALL_EVENT_HANDLER(CardCameraCreatedEvent);
	CALL_EVENT_HANDLER(CardCreatedEvent);
	CALL_EVENT_HANDLER(CardRemovedEvent);
	CALL_EVENT_HANDLER(CardStateChangedEvent);
	CALL_EVENT_HANDLER(CardUnassignedEvent);
	CALL_EVENT_HANDLER(DisconnectedFromServerEvent);
	CALL_EVENT_HANDLER(EntityIdMappingCreatedEvent);
	CALL_EVENT_HANDLER(EntityHoveredEvent);
	CALL_EVENT_HANDLER(EntityTappedEvent);
	CALL_EVENT_HANDLER(EntityUnhoveredEvent);
	CALL_EVENT_HANDLER(FightResolvedEvent);
	CALL_EVENT_HANDLER(MatchEndedEvent);
	CALL_EVENT_HANDLER(PlayerAddedEvent);
	CALL_EVENT_HANDLER(PositionChangedEvent);
	CALL_EVENT_HANDLER(PowerChangedEvent);
	CALL_EVENT_HANDLER(RenderTargetChangedEvent);
	CALL_EVENT_HANDLER(ShipDamagedEvent);
	CALL_EVENT_HANDLER(StructureChangedEvent);
}

EVENT_HANDLER_DEFINITION(CardLayoutSystem, CardAssignedEvent)
{
	auto clientAssignedCard = this->entityIdMapping->ServerToClientId(data.assignedCard);
	auto clientTargetCard = this->entityIdMapping->ServerToClientId(data.targetCard);

	// Assign card.
	this->currentAssignments.insert(std::pair<Entity, Entity>(clientAssignedCard, clientTargetCard));

	// Update layout.
	this->LayoutCards();
}

EVENT_HANDLER_DEFINITION(CardLayoutSystem, CardCameraCreatedEvent)
{
	this->cardCamera = data.entity;
}

EVENT_HANDLER_DEFINITION(CardLayoutSystem, EntityIdMappingCreatedEvent)
{
	this->entityIdMapping = data.entityIdMapping;
}

EVENT_HANDLER_DEFINITION(CardLayoutSystem, CardCreatedEvent)
{
	auto cardEntity = this->entityIdMapping->ServerToClientId(data.serverEntity);
	auto card = this->CreateCard(cardEntity);

	// Add to list.
	this->cards.push_back(card);

	// Update layout.
	this->LayoutCards();
}

EVENT_HANDLER_DEFINITION(CardLayoutSystem, CardStateChangedEvent)
{
	auto cardEntity = this->entityIdMapping->ServerToClientId(data.serverEntity);
	auto cardStateComponent = this->game->entityManager->GetComponent<CardStateComponent>(cardEntity, CardStateComponent::CardStateComponentType);
	cardStateComponent->cardState = data.newState;

	// Update layout.
	this->LayoutCards();
}

EVENT_HANDLER_DEFINITION(CardLayoutSystem, CardRemovedEvent)
{
	auto clientEntity = this->entityIdMapping->ServerToClientId(data.serverEntity);

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

EVENT_HANDLER_DEFINITION(CardLayoutSystem, CardUnassignedEvent)
{
	auto clientAssignedCard = this->entityIdMapping->ServerToClientId(data.assignedCard);

	// Remove assignment.
	auto assignment = this->currentAssignments.find(clientAssignedCard);

	if (assignment != this->currentAssignments.end())
	{
		this->currentAssignments.erase(assignment);
	}

	// Update layout.
	this->LayoutCards();
}

EVENT_HANDLER_DEFINITION(CardLayoutSystem, DisconnectedFromServerEvent)
{
	this->Reset();
}

EVENT_HANDLER_DEFINITION(CardLayoutSystem, EntityHoveredEvent)
{
	// Find hovered card.
	auto cardEntity = this->CardBackgroundToEntityId(data.entity);

	if (cardEntity != INVALID_ENTITY_ID)
	{
		this->cardDetailView = this->CreateCard(cardEntity);
	}

	// Update layout.
	this->LayoutCards();
}

EVENT_HANDLER_DEFINITION(CardLayoutSystem, EntityTappedEvent)
{
	// Find tapped card.
	auto cardEntity = this->CardBackgroundToEntityId(data.entity);

	if (cardEntity != INVALID_ENTITY_ID)
	{
		// Notify listeners.
		auto cardTappedEvent = std::make_shared<CardTappedEvent>(cardEntity);
		this->game->eventManager->QueueEvent(cardTappedEvent);
	}
}

EVENT_HANDLER_DEFINITION(CardLayoutSystem, EntityUnhoveredEvent)
{
	// Find unhovered card.
	auto cardEntity = this->CardBackgroundToEntityId(data.entity);

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

EVENT_HANDLER_DEFINITION(CardLayoutSystem, FightResolvedEvent)
{
	auto clientAssignedCard = this->entityIdMapping->ServerToClientId(data.assignedCard);

	// Remove assignment
	auto assignment = this->currentAssignments.find(clientAssignedCard);

	if (assignment != this->currentAssignments.end())
	{
		this->currentAssignments.erase(assignment);
	}

	// Update layout.
	this->LayoutCards();
}

EVENT_HANDLER_DEFINITION(CardLayoutSystem, MatchEndedEvent)
{
	this->Reset();
}

EVENT_HANDLER_DEFINITION(CardLayoutSystem, PlayerAddedEvent)
{
	auto player = std::make_shared<PlayerLayoutData>();
	player->playerEntity = data.serverEntity;

	this->players.push_back(player);
}

EVENT_HANDLER_DEFINITION(CardLayoutSystem, PositionChangedEvent)
{
	if (this->cardCamera == INVALID_ENTITY_ID || data.entity != this->cardCamera)
	{
		return;
	}

	this->LayoutCards();
}

EVENT_HANDLER_DEFINITION(CardLayoutSystem, PowerChangedEvent)
{
	auto card = this->ServerEntityToCard(data.entity);

	if (card != nullptr)
	{
		this->uiFactory->SetText(card->powerValueLabel, std::to_string(data.newPower));
	}
}

EVENT_HANDLER_DEFINITION(CardLayoutSystem, RenderTargetChangedEvent)
{
	this->d2dContext = data.d2dContext;

	this->LoadResources();
}

EVENT_HANDLER_DEFINITION(CardLayoutSystem, ShipDamagedEvent)
{
	Entity damageCardEntity = this->entityIdMapping->ServerToClientId(data.damageCard);
	Entity shipCardEntity = this->entityIdMapping->ServerToClientId(data.damagedShip);

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
		this->uiFactory->SetAnchor(damageCard->backgroundSprite, VerticalAnchor(VerticalAnchorType::VerticalCenter, this->damageCardOffsetY), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f, 0.0f), shipCard->backgroundSprite);
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
		this->uiFactory->SetAnchor(damageCard->backgroundSprite, VerticalAnchor(VerticalAnchorType::VerticalCenter, this->damageCardOffsetY), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), damageLayoutData->background);
		this->uiFactory->SetDepth(damageCard->panel, damageLayoutData->panelDepth);

		damageLayoutData->background = damageCard->backgroundSprite;
	}
}

EVENT_HANDLER_DEFINITION(CardLayoutSystem, StructureChangedEvent)
{
	auto card = this->ServerEntityToCard(data.entity);

	if (card != nullptr)
	{
		this->uiFactory->SetText(card->structureValueLabel, std::to_string(data.newStructure) + "%");
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
	this->uiFactory->SetPanel(card->backgroundSprite, card->panel);
	this->uiFactory->SetTappable(card->backgroundSprite, true);
	this->uiFactory->FinishUIWidget(card->backgroundSprite);

	// Name label.
	auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(card->cardEntity, CardComponent::CardComponentType);

	card->nameLabel = this->uiFactory->CreateLabel("Card_" + std::to_string(cardComponent->setIndex) + "_" + std::to_string(cardComponent->cardIndex));
	this->uiFactory->SetAnchor(card->nameLabel, VerticalAnchor(VerticalAnchorType::Top, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), card->backgroundSprite);
	this->uiFactory->SetColor(card->nameLabel, D2D1::ColorF(D2D1::ColorF::Black));
	this->uiFactory->SetPanel(card->nameLabel, card->panel);
	this->uiFactory->FinishUIWidget(card->nameLabel);

	// Card type label.
	auto cardType = StringToWString(CardTypeToString(cardComponent->cardType));

	card->cardTypeLabel = this->uiFactory->CreateLabel("Card_Type_" + CardTypeToString(cardComponent->cardType));
	this->uiFactory->SetAnchor(card->cardTypeLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), card->backgroundSprite);
	this->uiFactory->SetColor(card->cardTypeLabel, D2D1::ColorF(D2D1::ColorF::Black));
	this->uiFactory->SetPanel(card->cardTypeLabel, card->panel);
	this->uiFactory->FinishUIWidget(card->cardTypeLabel);

	// Threat label.
	auto threatComponent = this->game->entityManager->GetComponent<ThreatComponent>(card->cardEntity, ThreatComponent::ThreatComponentType);

	if (threatComponent != nullptr)
	{
		card->threatLabel = this->uiFactory->CreateLabel();
		this->uiFactory->SetAnchor(card->threatLabel, VerticalAnchor(VerticalAnchorType::Top, 0.0f), HorizontalAnchor(HorizontalAnchorType::Left, 0.0f), card->backgroundSprite);
		this->uiFactory->SetColor(card->threatLabel, D2D1::ColorF(D2D1::ColorF::Black));
		this->uiFactory->SetPanel(card->threatLabel, card->panel);
		this->uiFactory->SetText(card->threatLabel, "[" + std::to_string(threatComponent->threat) + "]");
		this->uiFactory->FinishUIWidget(card->threatLabel);
	}

	// Power label.
	auto powerComponent = this->game->entityManager->GetComponent<PowerComponent>(card->cardEntity, PowerComponent::PowerComponentType);

	if (powerComponent != nullptr)
	{
		card->powerLabel = this->uiFactory->CreateLabel("Card_Property_Power");
		this->uiFactory->SetAnchor(card->powerLabel, VerticalAnchor(VerticalAnchorType::Bottom, 0.0f), HorizontalAnchor(HorizontalAnchorType::Left, 0.0f), card->backgroundSprite);
		this->uiFactory->SetColor(card->powerLabel, D2D1::ColorF(D2D1::ColorF::Black));
		this->uiFactory->SetPanel(card->powerLabel, card->panel);
		this->uiFactory->FinishUIWidget(card->powerLabel);

		card->powerValueLabel = this->uiFactory->CreateLabel();
		this->uiFactory->SetAnchor(card->powerValueLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::Left, powerLabelOffsetX), card->powerLabel);
		this->uiFactory->SetColor(card->powerValueLabel, D2D1::ColorF(D2D1::ColorF::Black));
		this->uiFactory->SetPanel(card->powerValueLabel, card->panel);
		this->uiFactory->SetText(card->powerValueLabel, std::to_string(powerComponent->power));
		this->uiFactory->FinishUIWidget(card->powerValueLabel);
	}

	// Structure label.
	auto structureComponent = this->game->entityManager->GetComponent<StructureComponent>(card->cardEntity, StructureComponent::StructureComponentType);

	if (structureComponent != nullptr)
	{
		card->structureLabel = this->uiFactory->CreateLabel("Card_Property_Structure");
		this->uiFactory->SetAnchor(card->structureLabel, VerticalAnchor(VerticalAnchorType::Bottom, 0.0f), HorizontalAnchor(HorizontalAnchorType::Right, -structureLabelOffsetX), card->backgroundSprite);
		this->uiFactory->SetColor(card->structureLabel, D2D1::ColorF(D2D1::ColorF::Black));
		this->uiFactory->SetPanel(card->structureLabel, card->panel);
		this->uiFactory->FinishUIWidget(card->structureLabel);

		card->structureValueLabel = this->uiFactory->CreateLabel();
		this->uiFactory->SetAnchor(card->structureValueLabel, VerticalAnchor(VerticalAnchorType::Bottom, 0.0f), HorizontalAnchor(HorizontalAnchorType::Right, 0.0f), card->backgroundSprite);
		this->uiFactory->SetColor(card->structureValueLabel, D2D1::ColorF(D2D1::ColorF::Black));
		this->uiFactory->SetPanel(card->structureValueLabel, card->panel);
		this->uiFactory->SetText(card->structureValueLabel, std::to_string(structureComponent->structure) + "%");
		this->uiFactory->FinishUIWidget(card->structureValueLabel);
	}

	// Ability label.
	std::string cardText = "Card_" + std::to_string(cardComponent->setIndex) + "_" + std::to_string(cardComponent->cardIndex) + "_Text";

	card->abilityLabel = this->uiFactory->CreateLabel(cardText, 0.08f);
	this->uiFactory->SetAnchor(card->abilityLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, abilityLabelOffsetY), HorizontalAnchor(HorizontalAnchorType::Left, abilityLabelOffsetX), card->backgroundSprite);
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
	// Get card camera position.
	auto cameraPosition = this->game->entityManager->GetComponent<PositionComponent>(this->cardCamera, PositionComponent::PositionComponentType);

	// Count cards.
	for (auto iterator = this->players.begin(); iterator != this->players.end(); iterator++)
	{
		auto player = *iterator;
		player->handCards = 0;
		player->unassignedCards = 0;
	}

	int assignedPlayerCards = 0;
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
			else
			{
				// Get player.
				auto player = this->GetPlayer(ownerComponent->owner);

				// Update playe card count.
				if (cardStateComponent->cardState == CardState::Hand)
				{
					// Card is in hand.
					player->handCards++;
				}
				else
				{
					// Card is not assigned.
					player->unassignedCards++;
				}
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
	float unassignedCardOffset = assignedPlayerCards * cardOffset;

	int playerIndex = 0;
	for (auto iterator = this->players.begin(); iterator != this->players.end(); iterator++)
	{
		auto player = *iterator;

		// Center unassigned player cards to the right.
		player->unassignedCardPositionX = -(player->unassignedCards - 1) * cardOffset / 2 + unassignedCardOffset;

		// Center player hand cards.
		player->handCardPositionX = -(player->handCards - 1) * cardOffset / 2;

		// Apply card camera.
		player->unassignedCardPositionX -= cameraPosition->position.x;
		player->handCardPositionX -= cameraPosition->position.x;

		// Distribute players across screens.
		player->unassignedCardPositionX += playerIndex;
		player->handCardPositionX += playerIndex;

		++playerIndex;
	}

	// Align assigned enemy cards left. Assigned player cards will be anchored to their respective enemies.
	float assignedEnemyCardPositionX = firstAssignedCardPositionX;

	// Center unassigned player cards to the right.
	float unassignedEnemyCardPositionX = -(unassignedEnemyCards - 1) * cardOffset / 2 + unassignedCardOffset;

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
			else
			{
				// Get player.
				auto player = this->GetPlayer(ownerComponent->owner);

				if (cardStateComponent->cardState == CardState::Hand)
				{
					// Card is in hand.
					this->uiFactory->SetAnchor(card->backgroundSprite, VerticalAnchor(VerticalAnchorType::Bottom, this->playerHandPositionY), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, player->handCardPositionX), 0);
					player->handCardPositionX += cardOffset;
				}
				else
				{
					// Card is not assigned.
					this->uiFactory->SetAnchor(card->backgroundSprite, VerticalAnchor(VerticalAnchorType::VerticalCenter, this->playerCardPositionY), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, player->unassignedCardPositionX), 0);
					player->unassignedCardPositionX += cardOffset;
				}
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
					assignedEnemyCardPositionX += cardOffset;
				}
				else
				{
					this->uiFactory->SetAnchor(card->backgroundSprite, VerticalAnchor(VerticalAnchorType::VerticalCenter, this->enemyCardPositionY), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, unassignedEnemyCardPositionX), 0);
					unassignedEnemyCardPositionX += cardOffset;
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
	this->game->entityManager->RemoveEntity(card->abilityLabel);
	this->game->entityManager->RemoveEntity(card->backgroundSprite);
	this->game->entityManager->RemoveEntity(card->cardEntity);
	this->game->entityManager->RemoveEntity(card->cardTypeLabel);
	this->game->entityManager->RemoveEntity(card->nameLabel);
	this->game->entityManager->RemoveEntity(card->panel);
	this->game->entityManager->RemoveEntity(card->powerLabel);
	this->game->entityManager->RemoveEntity(card->powerValueLabel);
	this->game->entityManager->RemoveEntity(card->structureLabel);
	this->game->entityManager->RemoveEntity(card->structureValueLabel);
	this->game->entityManager->RemoveEntity(card->threatLabel);
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

void CardLayoutSystem::Reset()
{
	for (auto iterator = this->cards.begin(); iterator != this->cards.end(); iterator++)
	{
		auto card = *iterator;
		this->RemoveCardEntity(card);
	}

	if (this->cardDetailView != nullptr)
	{
		// Remove card.
		this->RemoveCardEntity(this->cardDetailView);
		this->cardDetailView = nullptr;
	}

	this->cards.clear();
	this->currentAssignments.clear();
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

std::shared_ptr<PlayerLayoutData> CardLayoutSystem::GetPlayer(Entity entity)
{
	// Get player.
	for (auto iterator = this->players.begin(); iterator != this->players.end(); iterator++)
	{
		auto player = *iterator;

		if (player->playerEntity == entity)
		{
			return player;
		}
	}

	return nullptr;
}