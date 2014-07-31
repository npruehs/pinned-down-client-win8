#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct EntityInitializedEvent : public PinnedDownCore::Event
		{
			static const HashedString EntityInitializedEventType;

			const HashedString & GetEventType() const
			{
				return EntityInitializedEventType;
			}

			int entityId;

			explicit EntityInitializedEvent(int entityId)
			{
				this->entityId = entityId;
			}
		};
	}
}