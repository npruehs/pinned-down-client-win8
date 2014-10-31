#include "pch.h"

#include "Game.h"

#include "FightSystem.h"

#include "Actions\ResolveFightAction.h"

#include "Components\CardComponent.h"
#include "Components\CardStateComponent.h"
#include "Components\OwnerComponent.h"


using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;


FightSystem::FightSystem() :
	currentUIMode(UI::UIMode::DefaultUIMode)
{
}

void FightSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, CardTappedEvent::CardTappedEventType);
	this->game->eventManager->AddListener(this, EntityIdMappingCreatedEvent::EntityIdMappingCreatedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
	this->game->eventManager->AddListener(this, UIModeChangedEvent::UIModeChangedEventType);
}

void FightSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == CardTappedEvent::CardTappedEventType)
	{
		CardTappedEvent& cardTappedEvent = static_cast<CardTappedEvent&>(newEvent);
		this->OnCardTapped(cardTappedEvent);
	}
	else if (newEvent.GetEventType() == EntityIdMappingCreatedEvent::EntityIdMappingCreatedEventType)
	{
		EntityIdMappingCreatedEvent& entityIdMappingCreatedEvent = static_cast<EntityIdMappingCreatedEvent&>(newEvent);
		this->OnEntityIdMappingCreated(entityIdMappingCreatedEvent);
	}
	else if (newEvent.GetEventType() == TurnPhaseChangedEvent::TurnPhaseChangedEventType)
	{
		TurnPhaseChangedEvent& turnPhaseChangedEvent = static_cast<TurnPhaseChangedEvent&>(newEvent);
		this->OnTurnPhaseChanged(turnPhaseChangedEvent);
	}
	else if (newEvent.GetEventType() == UIModeChangedEvent::UIModeChangedEventType)
	{
		UIModeChangedEvent& uiModeChangedEvent = static_cast<UIModeChangedEvent&>(newEvent);
		this->OnUIModeChanged(uiModeChangedEvent);
	}
}

void FightSystem::OnCardTapped(CardTappedEvent& cardTappedEvent)
{
	if (this->turnPhase != TurnPhase::Fight || this->currentUIMode != UI::UIMode::DefaultUIMode)
	{
		return;
	}

	auto tappedCard = cardTappedEvent.entity;

	auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(tappedCard, CardComponent::CardComponentType);
	auto cardStateComponent = this->game->entityManager->GetComponent<CardStateComponent>(tappedCard, CardStateComponent::CardStateComponentType);
	auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(tappedCard, OwnerComponent::OwnerComponentType);
	
	if (ownerComponent == nullptr || ownerComponent->owner == INVALID_ENTITY_ID)
	{
		return;
	}

	if (cardComponent == nullptr || cardComponent->cardType != CardType::Starship)
	{
		return;
	}

	if (cardStateComponent == nullptr || cardStateComponent->cardState != CardState::InPlay)
	{
		return;
	}

	// Notify server.
	auto tappedCardServer = this->entityIdMapping->ClientToServerId(tappedCard);

	auto resolveFightAction = std::make_shared<ResolveFightAction>(tappedCardServer);
	this->game->eventManager->QueueEvent(resolveFightAction);
}

void FightSystem::OnEntityIdMappingCreated(EntityIdMappingCreatedEvent& entityIdMappingCreatedEvent)
{
	this->entityIdMapping = entityIdMappingCreatedEvent.entityIdMapping;
}

void FightSystem::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	this->turnPhase = turnPhaseChangedEvent.newTurnPhase;
}

void FightSystem::OnUIModeChanged(UIModeChangedEvent& uiModeChangedEvent)
{
	this->currentUIMode = uiModeChangedEvent.newMode;
}
