#include "pch.h"

#include "Game.h"

#include "FightSystem.h"

#include "Actions\ResolveFightAction.h"

#include "Components\OwnerComponent.h"


using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;


FightSystem::FightSystem()
{
}

void FightSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, CardTappedEvent::CardTappedEventType);
	this->game->eventManager->AddListener(this, EntityIdMappingCreatedEvent::EntityIdMappingCreatedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
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
}

void FightSystem::OnCardTapped(CardTappedEvent& cardTappedEvent)
{
	if (this->turnPhase != TurnPhase::Fight)
	{
		return;
	}

	auto tappedCard = cardTappedEvent.entity;
	auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(tappedCard, OwnerComponent::OwnerComponentType);

	if (ownerComponent != nullptr && ownerComponent->owner != INVALID_ENTITY_ID)
	{
		// Notify server.
		auto tappedCardServer = this->entityIdMapping->ClientToServerId(tappedCard);

		auto resolveFightAction = std::make_shared<ResolveFightAction>(tappedCardServer);
		this->game->eventManager->QueueEvent(resolveFightAction);
	}
}

void FightSystem::OnEntityIdMappingCreated(EntityIdMappingCreatedEvent& entityIdMappingCreatedEvent)
{
	this->entityIdMapping = entityIdMappingCreatedEvent.entityIdMapping;
}

void FightSystem::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	this->turnPhase = turnPhaseChangedEvent.newTurnPhase;
}
