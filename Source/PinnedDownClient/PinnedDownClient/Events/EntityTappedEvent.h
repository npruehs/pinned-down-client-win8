#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct EntityTappedEvent : public PinnedDownCore::Event
		{
			static const HashedString EntityTappedEventType;

			const HashedString & GetEventType() const
			{
				return EntityTappedEventType;
			}

			int entityId;

			explicit EntityTappedEvent(int entityId)
			{
				this->entityId = entityId;
			}
		};
	}
}