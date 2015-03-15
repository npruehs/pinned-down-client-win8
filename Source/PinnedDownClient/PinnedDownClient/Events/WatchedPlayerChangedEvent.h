#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct WatchedPlayerChangedEvent : public PinnedDownCore::Event
		{
			static const HashedString WatchedPlayerChangedEventType;

			const HashedString & GetEventType() const
			{
				return WatchedPlayerChangedEventType;
			}

			Entity serverEntity;
			std::string playerName;

			explicit WatchedPlayerChangedEvent(Entity serverEntity, std::string playerName)
			{
				this->serverEntity = serverEntity;
				this->playerName = playerName;
			}

			std::string ToString() const
			{
				return std::string("Watched player changed: " + this->playerName);
			}
		};
	}
}