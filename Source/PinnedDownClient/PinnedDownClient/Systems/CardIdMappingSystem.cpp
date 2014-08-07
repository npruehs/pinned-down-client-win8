#include "pch.h"

#include "Game.h"
#include "Event.h"

#include "Events\CardIdMappingCreatedEvent.h"

#include "Systems\CardIdMappingSystem.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Util;



CardIdMappingSystem::CardIdMappingSystem()
{
}

void CardIdMappingSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);
	
	// Setup card mapping.
	this->cardFactory = std::make_shared<CardFactory>(this->game);
	this->cardIdMapping = std::make_shared<CardIdMapping>();

	auto cardIdMappingCreatedEvent = std::make_shared<CardIdMappingCreatedEvent>(this->cardIdMapping);
	this->game->eventManager->QueueEvent(cardIdMappingCreatedEvent);

	// Register listener.
	this->game->eventManager->AddListener(this, CardCreatedEvent::CardCreatedEventType);
}

void CardIdMappingSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == CardCreatedEvent::CardCreatedEventType)
	{
		CardCreatedEvent& cardCreatedEvent = static_cast<CardCreatedEvent&>(newEvent);
		this->OnCardCreated(cardCreatedEvent);
	}
}

void CardIdMappingSystem::OnCardCreated(CardCreatedEvent& cardCreatedEvent)
{
	// Create card copy on client.
	Entity clientEntity = this->cardFactory->CreateCard(cardCreatedEvent.setIndex, cardCreatedEvent.cardIndex);

	// Update mapping.
	this->cardIdMapping->MapCardId(cardCreatedEvent.serverEntity, clientEntity);
}
