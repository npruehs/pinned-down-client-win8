#include "pch.h"
#include "EntityIdMapping.h"

using namespace PinnedDownClient::Util;


EntityIdMapping::EntityIdMapping()
	: serverToClientIdMap(std::make_shared<BidirectionalMap<Entity, Entity>>())
{
}

void EntityIdMapping::Clear()
{
	this->serverToClientIdMap->Clear();
}

void EntityIdMapping::MapEntityId(Entity serverEntity, Entity clientEntity)
{
	this->serverToClientIdMap->Add(serverEntity, clientEntity);
}

Entity EntityIdMapping::ServerToClientId(Entity serverEntity)
{
	return this->serverToClientIdMap->GetValueOrDefault(serverEntity, INVALID_ENTITY_ID);
}

Entity EntityIdMapping::ClientToServerId(Entity clientEntity)
{
	return this->serverToClientIdMap->GetKeyOrDefault(clientEntity, INVALID_ENTITY_ID);
}