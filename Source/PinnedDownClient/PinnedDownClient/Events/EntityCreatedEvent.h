#pragma once

#include "Core\Event.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct EntityCreatedEvent : public PinnedDownClient::Core::Event
		{
			static const HashedString EntityCreatedEventType;

			const HashedString & GetEventType() const
			{
				return EntityCreatedEventType;
			}

			int entityId;

			explicit EntityCreatedEvent(int entityId)
			{
				this->entityId = entityId;
			}
		};
	}
}