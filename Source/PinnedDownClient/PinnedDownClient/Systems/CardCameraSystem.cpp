#include "pch.h"

#include <math.h>

#include "Game.h"
#include "Event.h"
#include "EntityManager.h"

#include "Source\Features\Layout\Components\PositionComponent.h"

#include "Events\CardCameraCreatedEvent.h"
#include "Events\PositionChangedEvent.h"
#include "Events\WatchedPlayerChangedEvent.h"

#include "Systems\CardCameraSystem.h"

#include "Util\MathUtils.h"


using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Features::Layout;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Util;


CardCameraSystem::CardCameraSystem()
	: cardCameraPositionX(0.0f),
	players(0)
{
}

void CardCameraSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, PlayerAddedEvent::PlayerAddedEventType);
	this->game->eventManager->AddListener(this, PointerDraggedEvent::PointerDraggedEventType);

	this->CreateCamera();
}

void CardCameraSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(PlayerAddedEvent);
	CALL_EVENT_HANDLER(PointerDraggedEvent);
}

EVENT_HANDLER_DEFINITION(CardCameraSystem, PlayerAddedEvent)
{
	// Add new player.
	auto player = std::make_shared<PlayerData>();
	player->playerName = data.playerName;
	player->serverEntity = data.serverEntity;

	this->players.push_back(player);

	// Check if first player.
	if (this->watchedPlayer == nullptr)
	{
		this->SetWatchedPlayer(player);
	}

	// Extend camera bounds.
	this->cardCameraPositionXMax = (float)(this->players.size() - 1);
}

EVENT_HANDLER_DEFINITION(CardCameraSystem, PointerDraggedEvent)
{
	// Update position.
	auto positionComponent = this->game->entityManager->GetComponent<PositionComponent>(this->camera, PositionComponent::PositionComponentType);
	auto oldPosition = positionComponent->position;

	positionComponent->position.x -= data.delta.x / this->designWidth;
	positionComponent->position.x = Clamp(positionComponent->position.x, 0.0f, this->cardCameraPositionXMax);

	if (positionComponent->position.x != oldPosition.x)
	{
		// Notify listeners.
		auto positionChangedEvent = std::make_shared<PositionChangedEvent>(this->camera, positionComponent->position);
		this->game->eventManager->QueueEvent(positionChangedEvent);
	}

	// Check watched player.
	auto playerIntervalSize = this->cardCameraPositionXMax / this->players.size();
	auto newPlayerIndex = (int)floor(positionComponent->position.x / playerIntervalSize);
	newPlayerIndex = Clamp(newPlayerIndex, 0, this->players.size() - 1);

	auto playerIterator = this->players.begin();
	std::advance(playerIterator, newPlayerIndex);

	std::shared_ptr<PlayerData> newPlayer = *playerIterator;

	if (newPlayer->serverEntity != this->watchedPlayer->serverEntity)
	{
		this->SetWatchedPlayer(newPlayer);
	}
}

void CardCameraSystem::CreateCamera()
{
	// Create camera.
	this->camera = this->game->entityManager->CreateEntity();

	auto positionComponent = std::make_shared<PositionComponent>();
	this->game->entityManager->AddComponent(this->camera, positionComponent);

	// Notify listeners.
	auto cardCameraCreatedEvent = std::make_shared<CardCameraCreatedEvent>(this->camera);
	this->game->eventManager->QueueEvent(cardCameraCreatedEvent);
}

void CardCameraSystem::SetWatchedPlayer(std::shared_ptr<PlayerData> newPlayer)
{
	this->watchedPlayer = newPlayer;

	// Notify listeners.
	auto watchedPlayerChangedEvent = std::make_shared<WatchedPlayerChangedEvent>(this->watchedPlayer->serverEntity, this->watchedPlayer->playerName);
	this->game->eventManager->QueueEvent(watchedPlayerChangedEvent);
}