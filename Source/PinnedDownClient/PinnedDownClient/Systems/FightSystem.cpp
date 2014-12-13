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
	CALL_EVENT_HANDLER(CardTappedEvent);
	CALL_EVENT_HANDLER(EntityIdMappingCreatedEvent);
	CALL_EVENT_HANDLER(TurnPhaseChangedEvent);
	CALL_EVENT_HANDLER(UIModeChangedEvent);
}

EVENT_HANDLER_DEFINITION(FightSystem, CardTappedEvent)
{
	if (this->turnPhase != TurnPhase::Fight || this->currentUIMode != UI::UIMode::DefaultUIMode)
	{
		return;
	}

	auto tappedCard = data.entity;

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

EVENT_HANDLER_DEFINITION(FightSystem, EntityIdMappingCreatedEvent)
{
	this->entityIdMapping = data.entityIdMapping;
}

EVENT_HANDLER_DEFINITION(FightSystem, TurnPhaseChangedEvent)
{
	this->turnPhase = data.newTurnPhase;
}

EVENT_HANDLER_DEFINITION(FightSystem, UIModeChangedEvent)
{
	this->currentUIMode = data.newMode;
}
