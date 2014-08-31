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
}

void EntityIdMappingSystem::OnEvent(Event & newEvent)
{
}