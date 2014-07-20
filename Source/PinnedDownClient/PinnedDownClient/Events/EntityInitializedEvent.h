#pragma once

#include "Core\Event.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct EntityInitializedEvent : public PinnedDownClient::Core::Event
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