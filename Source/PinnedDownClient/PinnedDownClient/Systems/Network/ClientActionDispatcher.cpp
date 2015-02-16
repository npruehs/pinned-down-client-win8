#include "pch.h"
#include "Systems\Network\ClientActionDispatcher.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Systems::Network;

ClientActionDispatcher::ClientActionDispatcher(Game* game, std::shared_ptr<ClientActionWriter> clientActionWriter)
{
	this->game = game;
	this->clientActionWriter = clientActionWriter;

	this->game->eventManager->AddListener(this);
}

ClientActionDispatcher::~ClientActionDispatcher()
{
	this->game->eventManager->RemoveListener(this);
}

void ClientActionDispatcher::OnEvent(Event& newEvent)
{
	if (newEvent.GetNetRole() == NetRole::Server)
	{
		this->clientActionWriter->WriteClientAction(newEvent);
	}
}
