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
	CALL_EVENT_HANDLER(CardAssignedEvent);
	CALL_EVENT_HANDLER(CardTappedEvent);
	CALL_EVENT_HANDLER(EntityIdMappingCreatedEvent);
	CALL_EVENT_HANDLER(TurnPhaseChangedEvent);
}

EVENT_HANDLER_DEFINITION(AssignmentSystem, CardAssignedEvent)
{
	auto assignedCardClient = this->entityIdMapping->ServerToClientId(data.assignedCard);

	if (assignedCardClient == this->selectedCard)
	{
		// Deselect current card.
		this->DeselectCard();
	}
}

EVENT_HANDLER_DEFINITION(AssignmentSystem, CardTappedEvent)
{
	if (this->turnPhase != TurnPhase::Assignment)
	{
		return;
	}

	auto tappedCard = data.entity;

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
			else
			{
				// Deselect current card.
				this->DeselectCard();
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

EVENT_HANDLER_DEFINITION(AssignmentSystem, EntityIdMappingCreatedEvent)
{
	this->entityIdMapping = data.entityIdMapping;
}

EVENT_HANDLER_DEFINITION(AssignmentSystem, TurnPhaseChangedEvent)
{
	if (this->turnPhase == TurnPhase::Assignment && this->selectedCard != INVALID_ENTITY_ID)
	{
		this->DeselectCard();
	}

	this->turnPhase = data.newTurnPhase;
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

