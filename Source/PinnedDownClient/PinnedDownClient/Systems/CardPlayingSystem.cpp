#include "pch.h"

#include "Game.h"

#include "CardPlayingSystem.h"

#include "Actions\PlayCardAction.h"

#include "Components\CardStateComponent.h"
#include "Components\OwnerComponent.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;


CardPlayingSystem::CardPlayingSystem()
{
}

void CardPlayingSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, CardTappedEvent::CardTappedEventType);
	this->game->eventManager->AddListener(this, EntityIdMappingCreatedEvent::EntityIdMappingCreatedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void CardPlayingSystem::OnEvent(Event & newEvent)
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
}

void CardPlayingSystem::OnCardTapped(CardTappedEvent& cardTappedEvent)
{
	if (this->turnPhase != TurnPhase::Main)
	{
		return;
	}

	auto tappedCard = cardTappedEvent.entity;
	auto cardStateComponent = this->game->entityManager->GetComponent<CardStateComponent>(tappedCard, CardStateComponent::CardStateComponentType);
	auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(tappedCard, OwnerComponent::OwnerComponentType);

	if (ownerComponent != nullptr && cardStateComponent != nullptr)
	{
		// Check owner of tapped card.
		if (ownerComponent->owner != INVALID_ENTITY_ID)
		{
			// Check if in hand.
			if (cardStateComponent->cardState == CardState::Hand)
			{
				// Notify server.
				auto tappedCardServer = this->entityIdMapping->ClientToServerId(tappedCard);

				auto playCardAction = std::make_shared<PlayCardAction>(tappedCardServer);
				this->game->eventManager->QueueEvent(playCardAction);
			}
		}
	}
}

void CardPlayingSystem::OnEntityIdMappingCreated(EntityIdMappingCreatedEvent& entityIdMappingCreatedEvent)
{
	this->entityIdMapping = entityIdMappingCreatedEvent.entityIdMapping;
}

void CardPlayingSystem::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	this->turnPhase = turnPhaseChangedEvent.newTurnPhase;
}
