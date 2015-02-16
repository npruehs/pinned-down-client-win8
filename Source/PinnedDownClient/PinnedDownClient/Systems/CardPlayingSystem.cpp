#include "pch.h"

#include "Game.h"

#include "CardPlayingSystem.h"

#include "Actions\PlayCardAction.h"

#include "Components\CardComponent.h"
#include "Components\CardStateComponent.h"
#include "Components\OwnerComponent.h"

#include "Events\CardSelectedEvent.h"
#include "Events\CardDeselectedEvent.h"
#include "Events\UIModeChangedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;


CardPlayingSystem::CardPlayingSystem() :
	selectedCard(INVALID_ENTITY_ID)
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
	CALL_EVENT_HANDLER(CardTappedEvent);
	CALL_EVENT_HANDLER(EntityIdMappingCreatedEvent);
	CALL_EVENT_HANDLER(TurnPhaseChangedEvent);
}

EVENT_HANDLER_DEFINITION(CardPlayingSystem, CardTappedEvent)
{
	auto tappedCard = data.entity;
	
	// Check if selecting a target.
	if (this->selectedCard != INVALID_ENTITY_ID)
	{
		if (tappedCard != this->selectedCard)
		{
			// Play card on target.
			auto cardToPlayServer = this->entityIdMapping->ClientToServerId(this->selectedCard);
			auto targetCardServer = this->entityIdMapping->ClientToServerId(tappedCard);

			auto playCardAction = std::make_shared<PlayCardAction>(cardToPlayServer, targetCardServer);
			this->game->eventManager->QueueEvent(playCardAction);
		}

		// Deselect current card - no matter if it's the tapped card itself, or if we've chosen a target.
		this->DeselectCard();
	}
	else
	{
		// No card selected. Take a closer look at tapped card, it should be a hand card.
		auto cardStateComponent = this->game->entityManager->GetComponent<CardStateComponent>(tappedCard, CardStateComponent::CardStateComponentType);

		if (cardStateComponent == nullptr || cardStateComponent->cardState != CardState::Hand)
		{
			// Early out.
			return;
		}

		// Card is in our hand, check if requires a target to play.
		auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(tappedCard, CardComponent::CardComponentType);

		if (cardComponent->cardType == CardType::Starship && this->turnPhase == TurnPhase::Main)
		{
			// Play card.
			auto tappedCardServer = this->entityIdMapping->ClientToServerId(tappedCard);

			auto playCardAction = std::make_shared<PlayCardAction>(tappedCardServer, INVALID_ENTITY_ID);
			this->game->eventManager->QueueEvent(playCardAction);
		}
		else if (cardComponent->cardType == CardType::Effect && this->turnPhase == TurnPhase::Fight)
		{
			// Prompt player to choose a target.
			this->SelectCard(tappedCard);
		}
	}
}

EVENT_HANDLER_DEFINITION(CardPlayingSystem, EntityIdMappingCreatedEvent)
{
	this->entityIdMapping = data.entityIdMapping;
}

EVENT_HANDLER_DEFINITION(CardPlayingSystem, TurnPhaseChangedEvent)
{
	this->turnPhase = data.newTurnPhase;
}

void CardPlayingSystem::SelectCard(Entity card)
{
	auto cardSelectedEvent = std::make_shared<CardSelectedEvent>(card);
	this->game->eventManager->QueueEvent(cardSelectedEvent);

	this->selectedCard = card;

	auto uiModeChangedEvent = std::make_shared<UIModeChangedEvent>(UI::UIMode::SelectCardTarget);
	this->game->eventManager->QueueEvent(uiModeChangedEvent);
}

void CardPlayingSystem::DeselectCard()
{
	auto cardDeselectedEvent = std::make_shared<CardDeselectedEvent>(this->selectedCard);
	this->game->eventManager->QueueEvent(cardDeselectedEvent);

	this->selectedCard = INVALID_ENTITY_ID;

	auto uiModeChangedEvent = std::make_shared<UIModeChangedEvent>(UI::UIMode::DefaultUIMode);
	this->game->eventManager->QueueEvent(uiModeChangedEvent);
}