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
	if (newEvent.GetEventType() == ClientIdMappingCreatedEvent::ClientIdMappingCreatedEventType)
	{
		ClientIdMappingCreatedEvent& clientIdMappingCreatedEvent = static_cast<ClientIdMappingCreatedEvent&>(newEvent);
		this->OnClientIdMappingCreated(clientIdMappingCreatedEvent);
	}
	if (newEvent.GetEventType() == PlayerAddedEvent::PlayerAddedEventType)
	{
		PlayerAddedEvent& playerAddedEvent = static_cast<PlayerAddedEvent&>(newEvent);
		this->OnPlayerAdded(playerAddedEvent);
	}
}

void PlayerSystem::OnClientIdMappingCreated(ClientIdMappingCreatedEvent& clientIdMappingCreatedEvent)
{
	this->clientIdMapping = clientIdMappingCreatedEvent.clientIdMapping;
}

void PlayerSystem::OnPlayerAdded(PlayerAddedEvent& playerAddedEvent)
{
	// Map player entity id.
	this->clientIdMapping->MapClientId(playerAddedEvent.clientId, playerAddedEvent.serverEntity);
}
