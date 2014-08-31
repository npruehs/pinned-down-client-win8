#include "pch.h"

#include "Game.h"
#include "Event.h"

#include "Events\ClientIdMappingCreatedEvent.h"

#include "Systems\ClientIdMappingSystem.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Util;



ClientIdMappingSystem::ClientIdMappingSystem()
{
}

void ClientIdMappingSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	// Setup client id mapping.
	this->clientIdMapping = std::make_shared<ClientIdMapping>();

	auto clientIdMappingCreatedEvent = std::make_shared<ClientIdMappingCreatedEvent>(this->clientIdMapping);
	this->game->eventManager->QueueEvent(clientIdMappingCreatedEvent);

	// Register listener.
	this->game->eventManager->AddListener(this, LoginSuccessEvent::LoginSuccessEventType);
}

void ClientIdMappingSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == LoginSuccessEvent::LoginSuccessEventType)
	{
		LoginSuccessEvent& loginSuccessEvent = static_cast<LoginSuccessEvent&>(newEvent);
		this->OnLoginSuccess(loginSuccessEvent);
	}
}

void ClientIdMappingSystem::OnLoginSuccess(LoginSuccessEvent& loginSuccessEvent)
{
	this->clientIdMapping->SetLocalClientId(loginSuccessEvent.clientId);
}