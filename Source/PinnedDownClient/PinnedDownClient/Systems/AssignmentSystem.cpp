#include "pch.h"

#include "Game.h"

#include "AssignmentSystem.h"

#include "Components\OwnerComponent.h"

#include "Events\CardDeselectedEvent.h"
#include "Events\CardSelectedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownNet::Components;


AssignmentSystem::AssignmentSystem() :
	selectedCard(INVALID_ENTITY_ID)
{
}

void AssignmentSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, CardTappedEvent::CardTappedEventType);
}

void AssignmentSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == CardTappedEvent::CardTappedEventType)
	{
		CardTappedEvent& cardTappedEvent = static_cast<CardTappedEvent&>(newEvent);
		this->OnCardTapped(cardTappedEvent);
	}
}

void AssignmentSystem::OnCardTapped(CardTappedEvent& cardTappedEvent)
{
	auto tappedCard = cardTappedEvent.entity;

	// Check whether tapped card can be selected.
	auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(tappedCard, OwnerComponent::OwnerComponentType);

	if (ownerComponent != nullptr && ownerComponent->owner != INVALID_ENTITY_ID)
	{
		// Check if already selected.
		if (this->selectedCard != tappedCard)
		{
			if (this->selectedCard != INVALID_ENTITY_ID)
			{
				// Deselect current card.
				auto cardDeselectedEvent = std::make_shared<CardDeselectedEvent>(this->selectedCard);
				this->game->eventManager->QueueEvent(cardDeselectedEvent);
			}

			// Select new card.
			auto cardSelectedEvent = std::make_shared<CardSelectedEvent>(tappedCard);
			this->game->eventManager->QueueEvent(cardSelectedEvent);

			this->selectedCard = tappedCard;
		}
	}
}