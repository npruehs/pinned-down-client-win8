#pragma once

#include <set>
#include <list>
#include <map>
#include <memory>
#include <string>
#include "Event.h"
#include "IEventListener.h"
#include "Util\HashedString.h"

namespace PinnedDownClient
{
	namespace Core
	{
		class EventManager
		{
		public:
			EventManager();

			void AddListener(std::shared_ptr<IEventListener> const & listener, PinnedDownClient::Util::HashedString const & eventType);
			void RemoveListener(std::shared_ptr<IEventListener> const & listener, PinnedDownClient::Util::HashedString const & eventType);
			void QueueEvent(std::shared_ptr<Event> const & newEvent);
			void Tick();

		private:
			// Set of registered event types.
			std::set<PinnedDownClient::Util::HashedString> eventTypes;

			// Maps event types to listeners.
			std::map<unsigned long, std::list<std::shared_ptr<IEventListener>>> listeners;

			// Events that are currently being processed.
			std::list<std::shared_ptr<Event>> currentEvents;

			// New events to be processed soon.
			std::list<std::shared_ptr<Event>> newEvents;
		};
	}
}
