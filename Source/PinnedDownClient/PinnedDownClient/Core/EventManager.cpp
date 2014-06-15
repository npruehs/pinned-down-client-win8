#include "pch.h"
#include "EventManager.h"
#include "GameException.h"

using namespace PinnedDownClient::Core;

EventManager::EventManager()
{
}

void EventManager::AddListener(std::shared_ptr<IEventListener> const & listener, PinnedDownClient::Util::HashedString const & eventType)
{
	// Get the event type entry in the listeners map.
	std::map<unsigned long, std::list<std::shared_ptr<IEventListener>>>::iterator iterator = this->listeners.find(eventType.getHash());
	std::list<std::shared_ptr<IEventListener>> eventListeners;

	if (iterator != this->listeners.end())
	{
		eventListeners = iterator->second;
	}
	else
	{
		// Add new entry to listeners map.
		eventListeners = std::list<std::shared_ptr<IEventListener>>();
		this->listeners.insert(std::pair<unsigned long, std::list<std::shared_ptr<IEventListener>>>(eventType.getHash(), eventListeners));
	}

	// Add listener.
	eventListeners.push_back(listener);
}

void EventManager::RemoveListener(std::shared_ptr<IEventListener> const & listener, PinnedDownClient::Util::HashedString const & eventType)
{
	// Find listener to remove.
	for (std::map<unsigned long, std::list<std::shared_ptr<IEventListener>>>::iterator it = this->listeners.begin(); it != this->listeners.end(); it++)
	{
		unsigned long const eventId = it->first;
		std::list<std::shared_ptr<IEventListener>> eventListeners = it->second;

		for (std::list<std::shared_ptr<IEventListener>>::iterator it2 = eventListeners.begin(); it2 != eventListeners.end(); it2++)
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

void EventManager::QueueEvent(std::shared_ptr<Event> const & newEvent)
{
	// Queue event.
	this->newEvents.push_back(newEvent);
}

void EventManager::Tick()
{
	while (this->newEvents.size() > 0)
	{
		// Move events from new event queue to current event queue.
		std::shared_ptr<Event> newEvent = this->newEvents.front();
		this->newEvents.pop_front();
		this->currentEvents.push_back(newEvent);

		// Process all events.
		while (this->currentEvents.size() > 0)
		{
			// Get next event to process.
			std::shared_ptr<Event> currentEvent = this->currentEvents.front();
			this->currentEvents.pop_front();

			PinnedDownClient::Util::HashedString const & eventType = currentEvent->GetEventType();

			// Get listeners for the event.
			std::map<unsigned long, std::list<std::shared_ptr<IEventListener>>>::const_iterator itListeners = this->listeners.find(eventType.getHash());

			if (itListeners != this->listeners.end())
			{
				std::list<std::shared_ptr<IEventListener>> const & eventListeners = itListeners->second;

				// Notify all listeners.
				for (std::list<std::shared_ptr<IEventListener>>::const_iterator it = eventListeners.begin(); it != eventListeners.end(); it++)
				{
					(*it)->OnEvent(*currentEvent);
				}
			}
		}
	}
}