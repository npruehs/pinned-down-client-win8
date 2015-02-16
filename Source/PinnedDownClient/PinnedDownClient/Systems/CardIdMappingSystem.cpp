#include "pch.h"

#include "Game.h"
#include "Event.h"

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
	
	// Setup card factory.
	this->cardFactory = std::make_shared<CardFactory>(this->game);

	// Register listener.
	this->game->eventManager->AddListener(this, CardCreatedEvent::CardCreatedEventType);
	this->game->eventManager->AddListener(this, EntityIdMappingCreatedEvent::EntityIdMappingCreatedEventType);
}

void CardIdMappingSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(CardCreatedEvent);
	CALL_EVENT_HANDLER(EntityIdMappingCreatedEvent);
}

EVENT_HANDLER_DEFINITION(CardIdMappingSystem, CardCreatedEvent)
{
	// Create card copy on client.
	Entity clientEntity = this->cardFactory->CreateCard(data.owner, data.setIndex, data.cardIndex, data.initialState);

	// Update mapping.
	this->entityIdMapping->MapEntityId(data.serverEntity, clientEntity);
}

EVENT_HANDLER_DEFINITION(CardIdMappingSystem, EntityIdMappingCreatedEvent)
{
	this->entityIdMapping = data.entityIdMapping;
}
