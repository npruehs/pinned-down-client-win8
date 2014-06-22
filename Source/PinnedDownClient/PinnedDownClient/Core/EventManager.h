#pragma once

#include <set>
#include <list>
#include <map>
#include <memory>
#include <string>
#include "Event.h"
#include "IEventListener.h"
#include "Util\HashedString.h"

using namespace PinnedDownClient::Util;

typedef std::shared_ptr<PinnedDownClient::Core::IEventListener> EventListenerPtr;
typedef std::shared_ptr<PinnedDownClient::Core::Event> EventPtr;

namespace PinnedDownClient
{
	namespace Core
	{
		class EventManager
		{
		public:
			EventManager();

			// Adds the passed listener for events of the specified type.
			void AddListener(EventListenerPtr const & listener, HashedString const & eventType);

			// Removes the passed listener for events of the specified type.
			void RemoveListener(EventListenerPtr const & listener, HashedString const & eventType);

			// Queues the specified event for processing in the next tick.
			void QueueEvent(EventPtr const & newEvent);

			// Raises the specified event to be handled immediately.
			void RaiseEvent(EventPtr const & newEvent);

			// Raises all queued events.
			void Tick();

		private:
			// Set of registered event types.
			std::set<HashedString> eventTypes;

			// Maps event types to listeners.
			std::map<unsigned long, std::list<EventListenerPtr>> listeners;

			// Events that are currently being processed.
			std::list<EventPtr> currentEvents;

			// New events to be processed soon.
			std::list<EventPtr> newEvents;
		};
	}
}
