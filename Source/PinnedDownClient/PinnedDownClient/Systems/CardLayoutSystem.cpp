#include "pch.h"

#include "Game.h"
#include "Event.h"
#include "EntityManager.h"

#include "Components\AffiliationComponent.h"
#include "Components\CardComponent.h"
#include "Components\CardUIComponent.h"
#include "Components\FlagshipComponent.h"
#include "Components\OwnerComponent.h"
#include "Components\PowerComponent.h"
#include "Components\ThreatComponent.h"

#include "Events\CardTappedEvent.h"
#include "Events\DisconnectedFromServerEvent.h"

#include "Resources\PinnedDownResourceManager.h"

#include "Systems\CardLayoutSystem.h"

#include "Util\StringUtils.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Resources;
using namespace PinnedDownClient::Systems;
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
	this->game->eventManager->AddListener(this, CardRemovedEvent::CardRemovedEventType);
	this->game->eventManager->AddListener(this, DisconnectedFromServerEvent::DisconnectedFromServerEventType);
	this->game->eventManager->AddListener(this, EntityTappedEvent::EntityTappedEventType);
	this->game->eventManager->AddListener(this, EntityIdMappingCreatedEvent::EntityIdMappingCreatedEventType);
	this->game->eventManager->AddListener(this, FightResolvedEvent::FightResolvedEventType);
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
	else if (newEvent.GetEventType() == DisconnectedFromServerEvent::DisconnectedFromServerEventType)
	{
		this->OnDisconnectedFromServer();
	}
	else if (newEvent.GetEventType() == EntityIdMappingCreatedEvent::EntityIdMappingCreatedEventType)
	{
		EntityIdMappingCreatedEvent& entityIdMappingCreatedEvent = static_cast<EntityIdMappingCreatedEvent&>(newEvent);
		this->OnEntityIdMappingCreated(entityIdMappingCreatedEvent);
	}
	else if(newEvent.GetEventType() == EntityTappedEvent::EntityTappedEventType)
	{
		EntityTappedEvent& entityTappedEvent = static_cast<EntityTappedEvent&>(newEvent);
		this->OnEntityTapped(entityTappedEvent);
	}
	else if (newEvent.GetEventType() == FightResolvedEvent::FightResolvedEventType)
	{
		FightResolvedEvent& fightResolvedEvent = static_cast<FightResolvedEvent&>(newEvent);
		this->OnFightResolved(fightResolvedEvent);
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

	// Check for previous assignment.
	auto assignment = this->currentAssignments.find(clientAssignedCard);

	if (assignment != this->currentAssignments.end())
	{
		this->currentAssignments.erase(assignment);
	}

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
	auto card = std::make_shared<Card>();
	card->cardEntity = this->entityIdMapping->ServerToClientId(cardCreatedEvent.serverEntity);

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

	card->nameLabel = this->uiFactory->CreateLabel(L"Card " + std::to_wstring(cardComponent->cardIndex));
	this->uiFactory->SetAnchor(card->nameLabel, VerticalAnchor(VerticalAnchorType::Top, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), card->backgroundSprite);
	this->uiFactory->SetColor(card->nameLabel, D2D1::ColorF(D2D1::ColorF::Black));
	this->uiFactory->SetPanel(card->nameLabel, card->panel);
	this->uiFactory->FinishUIWidget(card->nameLabel);

	// Card type label.
	auto cardType = StringToWString(CardTypeToString(cardComponent->cardType));

	card->cardTypeLabel = this->uiFactory->CreateLabel(StringToWString(CardTypeToString(cardComponent->cardType)));
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

	card->powerLabel = this->uiFactory->CreateLabel(L"Power " + std::to_wstring(powerComponent->power));
	this->uiFactory->SetAnchor(card->powerLabel, VerticalAnchor(VerticalAnchorType::Bottom, 0.0f), HorizontalAnchor(HorizontalAnchorType::Left, 0.0f), card->backgroundSprite);
	this->uiFactory->SetColor(card->powerLabel, D2D1::ColorF(D2D1::ColorF::Black));
	this->uiFactory->SetPanel(card->powerLabel, card->panel);
	this->uiFactory->FinishUIWidget(card->powerLabel);

	// Ability label.
	auto flagshipComponent = this->game->entityManager->GetComponent<FlagshipComponent>(card->cardEntity, FlagshipComponent::FlagshipComponentType);

	card->abilityLabel = this->uiFactory->CreateLabel(flagshipComponent != nullptr ? L"FLAGSHIP." : L"");
	this->uiFactory->SetAnchor(card->abilityLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 30.0f), HorizontalAnchor(HorizontalAnchorType::Left, 20.0f), card->backgroundSprite);
	this->uiFactory->SetColor(card->abilityLabel, D2D1::ColorF(D2D1::ColorF::Black));
	this->uiFactory->SetPanel(card->abilityLabel, card->panel);
	this->uiFactory->FinishUIWidget(card->abilityLabel);

	// Add to list.
	this->cards.push_back(card);

	auto cardUiComponent = std::make_shared<CardUIComponent>();
	cardUiComponent->background = card->backgroundSprite;
	this->game->entityManager->AddComponent(card->cardEntity, cardUiComponent);

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
			// Remove card.
			this->RemoveCardEntity(card);
			this->cards.erase(iterator);
			break;
		}
	}
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

void CardLayoutSystem::OnEntityTapped(EntityTappedEvent& entityTappedEvent)
{
	// Find tapped card.
	for (auto iterator = this->cards.begin(); iterator != this->cards.end(); iterator++)
	{
		auto card = *iterator;

		if (card->backgroundSprite == entityTappedEvent.entity)
		{
			// Notify listeners.
			auto cardTappedEvent = std::make_shared<CardTappedEvent>(card->cardEntity);
			this->game->eventManager->QueueEvent(cardTappedEvent);
		}
	}
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

	// Anchor damage card.
	this->uiFactory->SetAnchor(damageCard->backgroundSprite, VerticalAnchor(VerticalAnchorType::VerticalCenter, this->damageCardOffset), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0), shipCard->backgroundSprite);
	this->uiFactory->SetDepth(damageCard->panel, -10);
}

void CardLayoutSystem::LayoutCards()
{
	// Count cards.
	int playerCards = 0;
	int enemyCards = 0;

	for (auto iterator = this->cards.begin(); iterator != this->cards.end(); iterator++)
	{
		auto card = *iterator;
		auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(card->cardEntity, CardComponent::CardComponentType);
		auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(card->cardEntity, OwnerComponent::OwnerComponentType);

		if (cardComponent->cardType != CardType::Starship)
		{
			continue;
		}

		if (ownerComponent->owner != INVALID_ENTITY_ID)
		{
			playerCards++;
		}
		else
		{
			enemyCards++;
		}
	}

	// Layout cards.
	float playerCardPositionX = -(playerCards - 1) * (cardWidth + cardOffset) / 2;
	float enemyCardPositionX = -(enemyCards - 1) * (cardWidth + cardOffset) / 2;

	for (auto iterator = this->cards.begin(); iterator != this->cards.end(); iterator++)
	{
		auto card = *iterator;
		auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(card->cardEntity, CardComponent::CardComponentType);
		auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(card->cardEntity, OwnerComponent::OwnerComponentType);

		if (cardComponent->cardType != CardType::Starship)
		{
			continue;
		}

		if (ownerComponent->owner != INVALID_ENTITY_ID)
		{
			// Player card - check if assigned.
			auto assignment = this->currentAssignments.find(card->cardEntity);

			if (assignment != this->currentAssignments.end())
			{
				// Card is assigned to an enemy.
				auto targetCard = assignment->second;
				auto targetCardUiComponent = this->game->entityManager->GetComponent<CardUIComponent>(targetCard, CardUIComponent::CardUIComponentType);

				this->uiFactory->SetAnchor(card->backgroundSprite, VerticalAnchor(VerticalAnchorType::VerticalCenter, this->assignedCardOffset), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), targetCardUiComponent->background);
			}
			else
			{
				// Card is not assigned.
				this->uiFactory->SetAnchor(card->backgroundSprite, VerticalAnchor(VerticalAnchorType::VerticalCenter, playerCardPositionY), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, playerCardPositionX), 0);
			}

			playerCardPositionX += cardWidth + cardOffset;
		}
		else
		{
			// Enemy card.
			this->uiFactory->SetAnchor(card->backgroundSprite, VerticalAnchor(VerticalAnchorType::VerticalCenter, enemyCardPositionY), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, enemyCardPositionX), 0);
			enemyCardPositionX += cardWidth + cardOffset;
		}
	}
}

void CardLayoutSystem::RemoveCardEntity(std::shared_ptr<Card> card)
{
	this->game->entityManager->RemoveEntity(card->backgroundSprite);
	this->game->entityManager->RemoveEntity(card->cardEntity);
	this->game->entityManager->RemoveEntity(card->cardTypeLabel);
	this->game->entityManager->RemoveEntity(card->nameLabel);
	this->game->entityManager->RemoveEntity(card->powerLabel);
	this->game->entityManager->RemoveEntity(card->threatLabel);
	this->game->entityManager->RemoveEntity(card->abilityLabel);
}