#include "pch.h"
#include "ClientIdMapping.h"

using namespace PinnedDownClient::Util;


bool ClientIdMapping::IsLocalPlayer(Entity playerEntity)
{
	auto iterator = this->playerEntityToClientIdMap.find(playerEntity);
	return iterator != this->playerEntityToClientIdMap.end() ? (iterator->second == this->localClientId) : false;
}

void ClientIdMapping::MapClientId(int clientId, Entity playerEntity)
{
	this->clientToPlayerEntityIdMap.insert(std::pair<int, Entity>(clientId, playerEntity));
	this->playerEntityToClientIdMap.insert(std::pair<Entity, int>(playerEntity, clientId));
}

int ClientIdMapping::GetLocalClientId()
{
	return this->localClientId;
}

void ClientIdMapping::SetLocalClientId(int localClientId)
{
	this->localClientId = localClientId;
}