#pragma once

#include "Core\Event.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct DisplayContentsInvalidatedEvent : public PinnedDownClient::Core::Event
		{
			static const HashedString DisplayContentsInvalidatedEventType;

			const HashedString & GetEventType() const
			{
				return DisplayContentsInvalidatedEventType;
			}
		};
	}
}