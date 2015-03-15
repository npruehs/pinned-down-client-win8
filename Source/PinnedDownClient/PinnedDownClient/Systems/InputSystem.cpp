#include "pch.h"

#include "Event.h"

#include "Events\PointerDraggedEvent.h"

#include "Systems\InputSystem.h"


using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Math;
using namespace PinnedDownClient::Systems;


InputSystem::InputSystem()
	: fingerDown(false)
{
}

void InputSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, PointerMovedEvent::PointerMovedEventType);
	this->game->eventManager->AddListener(this, PointerPressedEvent::PointerPressedEventType);
	this->game->eventManager->AddListener(this, PointerReleasedEvent::PointerReleasedEventType);
}

void InputSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(PointerMovedEvent);
	CALL_EVENT_HANDLER(PointerPressedEvent);
	CALL_EVENT_HANDLER(PointerReleasedEvent);
}

EVENT_HANDLER_DEFINITION(InputSystem, PointerMovedEvent)
{
	if (!this->fingerDown)
	{
		return;
	}

	auto delta = data.position - this->lastPosition;
	auto pointerDraggedEvent = std::make_shared<PointerDraggedEvent>(delta);
	this->game->eventManager->QueueEvent(pointerDraggedEvent);

	this->lastPosition = data.position;
}

EVENT_HANDLER_DEFINITION(InputSystem, PointerPressedEvent)
{
	this->fingerDown = true;
	this->lastPosition = data.position;
}

EVENT_HANDLER_DEFINITION(InputSystem, PointerReleasedEvent)
{
	this->fingerDown = false;
}
