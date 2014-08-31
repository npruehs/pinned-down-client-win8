#include "pch.h"
#include "EntityIdMapping.h"

using namespace PinnedDownClient::Util;


void EntityIdMapping::MapEntityId(Entity serverEntity, Entity clientEntity)
{
	this->clientToServerIdMap.insert(std::pair<Entity, Entity>(clientEntity, serverEntity));
	this->serverToClientIdMap.insert(std::pair<Entity, Entity>(serverEntity, clientEntity));
}

Entity EntityIdMapping::ServerToClientId(Entity serverEntity)
{
	auto iterator = this->serverToClientIdMap.find(serverEntity);
	return iterator != this->serverToClientIdMap.end() ? iterator->second : INVALID_ENTITY_ID;
}

Entity EntityIdMapping::ClientToServerId(Entity clientEntity)
{
	auto iterator = this->clientToServerIdMap.find(clientEntity);
	return iterator != this->clientToServerIdMap.end() ? iterator->second : INVALID_ENTITY_ID;
}