#include "pch.h"

#include "Game.h"
#include "Event.h"

#include "Systems\PlayerSystem.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Data;
using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems;


PlayerSystem::PlayerSystem()
{
}

void PlayerSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	// Create player factory.
	this->playerFactory = std::make_shared<PlayerFactory>(game);

	// Register listener.
	this->game->eventManager->AddListener(this, ClientIdMappingCreatedEvent::ClientIdMappingCreatedEventType);
	this->game->eventManager->AddListener(this, PlayerAddedEvent::PlayerAddedEventType);
}

void PlayerSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(ClientIdMappingCreatedEvent);
	CALL_EVENT_HANDLER(PlayerAddedEvent);
}

EVENT_HANDLER_DEFINITION(PlayerSystem, ClientIdMappingCreatedEvent)
{
	this->clientIdMapping = data.clientIdMapping;
}

EVENT_HANDLER_DEFINITION(PlayerSystem, PlayerAddedEvent)
{
	// Map player entity id.
	this->clientIdMapping->MapClientId(data.clientId, data.serverEntity);
}
