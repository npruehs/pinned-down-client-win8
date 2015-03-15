
#include "pch.h"
#include "EventLogger.h"

#include "EntityCreatedEvent.h"
#include "EntityInitializedEvent.h"
#include "EntityRemovedEvent.h"

#include "Events\PointerDraggedEvent.h"
#include "Events\PointerMovedEvent.h"
#include "Events\PositionChangedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;

EventLogger::EventLogger(std::shared_ptr<PinnedDownCore::Game> game) : game(game)
{
	this->game->eventManager->AddListener(this);
}

void EventLogger::OnEvent(Event & newEvent)
{
	// Reduce log spam.
	if (newEvent.GetEventType() == EntityCreatedEvent::EntityCreatedEventType ||
		newEvent.GetEventType() == EntityInitializedEvent::EntityInitializedEventType ||
		newEvent.GetEventType() == EntityRemovedEvent::EntityRemovedEventType ||
		newEvent.GetEventType() == PointerMovedEvent::PointerMovedEventType ||
		newEvent.GetEventType() == PointerDraggedEvent::PointerDraggedEventType ||
		newEvent.GetEventType() == PositionChangedEvent::PositionChangedEventType)
	{
		return;
	}

	// Log event.
	this->game->logger->LogDebug(newEvent.ToString());
}