#include "pch.h"
#include "ClientIdMapping.h"

using namespace PinnedDownClient::Util;


ClientIdMapping::ClientIdMapping()
	: clientToPlayerEntityIdMap(std::make_shared<BidirectionalMap<int, Entity>>()),
	localClientId(0)
{
}

void ClientIdMapping::Clear()
{
	this->localClientId = 0;
	this->clientToPlayerEntityIdMap->Clear();
}

bool ClientIdMapping::IsLocalPlayer(Entity playerEntity)
{
	auto clientId = this->clientToPlayerEntityIdMap->GetKeyOrDefault(playerEntity, -1);
	return clientId != -1 && clientId == this->localClientId;
}

void ClientIdMapping::MapClientId(int clientId, Entity playerEntity)
{
	this->clientToPlayerEntityIdMap->Add(clientId, playerEntity);
}

int ClientIdMapping::GetLocalClientId()
{
	return this->localClientId;
}

void ClientIdMapping::SetLocalClientId(int localClientId)
{
	this->localClientId = localClientId;
}