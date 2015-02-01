#include "pch.h"

#include "Game.h"
#include "Event.h"

#include "Events\EntityIdMappingCreatedEvent.h"

#include "Systems\EntityIdMappingSystem.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Util;


EntityIdMappingSystem::EntityIdMappingSystem()
{
}

void EntityIdMappingSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	// Setup entity mapping.
	this->entityIdMapping = std::make_shared<EntityIdMapping>();

	// Notify listeners.
	auto entityIdMappingCreatedEvent = std::make_shared<EntityIdMappingCreatedEvent>(this->entityIdMapping);
	this->game->eventManager->QueueEvent(entityIdMappingCreatedEvent);

	// Register listener.
	this->game->eventManager->AddListener(this, DisconnectedFromServerEvent::DisconnectedFromServerEventType);
	this->game->eventManager->AddListener(this, MatchEndedEvent::MatchEndedEventType);
}

void EntityIdMappingSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(DisconnectedFromServerEvent);
	CALL_EVENT_HANDLER(MatchEndedEvent);
}

EVENT_HANDLER_DEFINITION(EntityIdMappingSystem, DisconnectedFromServerEvent)
{
	this->entityIdMapping->Clear();
}

EVENT_HANDLER_DEFINITION(EntityIdMappingSystem, MatchEndedEvent)
{
	this->entityIdMapping->Clear();
}