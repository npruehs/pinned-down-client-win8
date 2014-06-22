#include "pch.h"
#include "EventManager.h"
#include "GameException.h"

using namespace PinnedDownClient::Core;
using namespace PinnedDownClient::Util;

EventManager::EventManager()
{
}

void EventManager::AddListener(EventListenerPtr const & listener, HashedString const & eventType)
{
	// Get the event type entry in the listeners map.
	std::map<unsigned long, std::list<EventListenerPtr>>::iterator iterator = this->listeners.find(eventType.getHash());

	if (iterator != this->listeners.end())
	{
		std::list<EventListenerPtr> & eventListeners = iterator->second;

		// Add listener.
		eventListeners.push_back(listener);
	}
	else
	{
		// Add new entry to listeners map.
		std::list<EventListenerPtr> eventListeners = std::list<EventListenerPtr>();
		eventListeners.push_back(listener);
		this->listeners.insert(std::pair<unsigned long, std::list<EventListenerPtr>>(eventType.getHash(), eventListeners));
	}
}

void EventManager::RemoveListener(EventListenerPtr const & listener, HashedString const & eventType)
{
	// Find listener to remove.
	for (std::map<unsigned long, std::list<EventListenerPtr>>::iterator it = this->listeners.begin(); it != this->listeners.end(); it++)
	{
		unsigned long const eventId = it->first;
		std::list<EventListenerPtr> eventListeners = it->second;

		for (std::list<EventListenerPtr>::iterator it2 = eventListeners.begin(); it2 != eventListeners.end(); it2++)
		{
			if (*it2 == listener)
			{
				// Remove listener.
				eventListeners.erase(it2);
				return;
			}
		}
	}
}

void EventManager::QueueEvent(EventPtr const & newEvent)
{
	// Queue event.
	this->newEvents.push_back(newEvent);
}

void EventManager::RaiseEvent(EventPtr const & newEvent)
{
	HashedString const & eventType = newEvent->GetEventType();
	unsigned long eventHash = eventType.getHash();

	// Get listeners for the event.
	std::map<unsigned long, std::list<EventListenerPtr>>::const_iterator itListeners = this->listeners.find(eventHash);

	if (itListeners != this->listeners.end())
	{
		std::list<EventListenerPtr> const & eventListeners = itListeners->second;

		// Notify all listeners.
		for (std::list<EventListenerPtr>::const_iterator it = eventListeners.begin(); it != eventListeners.end(); it++)
		{
			(*it)->OnEvent(*newEvent);
		}
	}
}


void EventManager::Tick()
{
	while (this->newEvents.size() > 0)
	{
		// Move events from new event queue to current event queue.
		EventPtr newEvent = this->newEvents.front();
		this->newEvents.pop_front();
		this->currentEvents.push_back(newEvent);

		// Process all events.
		while (this->currentEvents.size() > 0)
		{
			// Get next event to process.
			EventPtr currentEvent = this->currentEvents.front();
			this->currentEvents.pop_front();

			this->RaiseEvent(currentEvent);
		}
	}
}