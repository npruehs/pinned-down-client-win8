#include "pch.h"

#include "Game.h"

#include "AssignmentSystem.h"

#include "Actions\AssignCardAction.h"

#include "Components\CardComponent.h"
#include "Components\CardStateComponent.h"
#include "Components\OwnerComponent.h"

#include "Events\CardDeselectedEvent.h"
#include "Events\CardSelectedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;


AssignmentSystem::AssignmentSystem() :
	selectedCard(INVALID_ENTITY_ID)
{
}

void AssignmentSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, CardAssignedEvent::CardAssignedEventType);
	this->game->eventManager->AddListener(this, CardTappedEvent::CardTappedEventType);
	this->game->eventManager->AddListener(this, EntityIdMappingCreatedEvent::EntityIdMappingCreatedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void AssignmentSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == CardAssignedEvent::CardAssignedEventType)
	{
		CardAssignedEvent& cardAssignedEvent = static_cast<CardAssignedEvent&>(newEvent);
		this->OnCardAssigned(cardAssignedEvent);
	}
	else if (newEvent.GetEventType() == CardTappedEvent::CardTappedEventType)
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

void AssignmentSystem::OnCardAssigned(CardAssignedEvent& cardAssignedEvent)
{
	auto assignedCardClient = this->entityIdMapping->ServerToClientId(cardAssignedEvent.assignedCard);

	if (assignedCardClient == this->selectedCard)
	{
		// Deselect current card.
		this->DeselectCard();
	}
}

void AssignmentSystem::OnCardTapped(CardTappedEvent& cardTappedEvent)
{
	if (this->turnPhase != TurnPhase::Assignment)
	{
		return;
	}

	auto tappedCard = cardTappedEvent.entity;

	auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(tappedCard, CardComponent::CardComponentType);
	auto cardStateComponent = this->game->entityManager->GetComponent<CardStateComponent>(tappedCard, CardStateComponent::CardStateComponentType);
	auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(tappedCard, OwnerComponent::OwnerComponentType);

	if (cardComponent == nullptr || cardComponent->cardType != CardType::Starship)
	{
		return;
	}

	if (cardStateComponent == nullptr || cardStateComponent->cardState != CardState::InPlay)
	{
		return;
	}

	if (ownerComponent != nullptr)
	{
		// Check owner of tapped card.
		if (ownerComponent->owner != INVALID_ENTITY_ID)
		{
			// Player card selected - check if already selected.
			if (this->selectedCard != tappedCard)
			{
				if (this->selectedCard != INVALID_ENTITY_ID)
				{
					// Deselect current card.
					this->DeselectCard();
				}

				// Select new card.
				this->SelectCard(tappedCard);
			}
		}
		else
		{
			// Enemy card selected - check if player card can be assigned.
			if (this->selectedCard != INVALID_ENTITY_ID)
			{
				// Notify server.
				auto selectedCardServer = this->entityIdMapping->ClientToServerId(this->selectedCard);
				auto tappedCardServer = this->entityIdMapping->ClientToServerId(tappedCard);

				auto assignCardAction = std::make_shared<AssignCardAction>(selectedCardServer, tappedCardServer);
				this->game->eventManager->QueueEvent(assignCardAction);
			}
		}
	}
}

void AssignmentSystem::OnEntityIdMappingCreated(EntityIdMappingCreatedEvent& entityIdMappingCreatedEvent)
{
	this->entityIdMapping = entityIdMappingCreatedEvent.entityIdMapping;
}

void AssignmentSystem::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	this->turnPhase = turnPhaseChangedEvent.newTurnPhase;
}

void AssignmentSystem::SelectCard(Entity card)
{
	auto cardSelectedEvent = std::make_shared<CardSelectedEvent>(card);
	this->game->eventManager->QueueEvent(cardSelectedEvent);

	this->selectedCard = card;
}

void AssignmentSystem::DeselectCard()
{
	auto cardDeselectedEvent = std::make_shared<CardDeselectedEvent>(this->selectedCard);
	this->game->eventManager->QueueEvent(cardDeselectedEvent);

	this->selectedCard = INVALID_ENTITY_ID;
}

