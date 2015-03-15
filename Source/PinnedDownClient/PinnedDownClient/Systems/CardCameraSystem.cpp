#include "pch.h"

#include "Game.h"
#include "Event.h"
#include "EntityManager.h"

#include "Components\PositionComponent.h"

#include "Events\CardCameraCreatedEvent.h"
#include "Events\PositionChangedEvent.h"

#include "Systems\CardCameraSystem.h"

#include "Util\MathUtils.h"


using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Components;
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

void CardCameraSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(PlayerAddedEvent);
	CALL_EVENT_HANDLER(PointerDraggedEvent);
}

EVENT_HANDLER_DEFINITION(CardCameraSystem, PlayerAddedEvent)
{
	++this->players;
	this->cardCameraPositionXMax = (float)(this->players - 1);
}

EVENT_HANDLER_DEFINITION(CardCameraSystem, PointerDraggedEvent)
{
	// Update position.
	auto positionComponent = this->game->entityManager->GetComponent<PositionComponent>(this->camera, PositionComponent::PositionComponentType);
	auto oldPosition = positionComponent->position;

	positionComponent->position.x -= data.delta.x / this->designWidth;
	positionComponent->position.x = Clamp(positionComponent->position.x, 0, this->cardCameraPositionXMax);

	if (positionComponent->position.x != oldPosition.x)
	{
		// Notify listeners.
		auto positionChangedEvent = std::make_shared<PositionChangedEvent>(this->camera, positionComponent->position);
		this->game->eventManager->QueueEvent(positionChangedEvent);
	}
}