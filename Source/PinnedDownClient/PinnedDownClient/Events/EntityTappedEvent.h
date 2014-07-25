#pragma once

#include "Core\Event.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct EntityTappedEvent : public PinnedDownClient::Core::Event
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