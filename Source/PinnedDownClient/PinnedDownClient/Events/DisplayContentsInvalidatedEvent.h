#pragma once

#include "Core\Event.h"

namespace PinnedDownClient
{
	namespace Events
	{
		struct DisplayContentsInvalidatedEvent : public PinnedDownClient::Core::Event
		{
			static const PinnedDownClient::Util::HashedString DisplayContentsInvalidatedEventType;

			const PinnedDownClient::Util::HashedString & GetEventType() const
			{
				return DisplayContentsInvalidatedEventType;
			}
		};
	}
}