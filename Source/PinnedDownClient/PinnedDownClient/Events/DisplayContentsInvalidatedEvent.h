#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct DisplayContentsInvalidatedEvent : public PinnedDownCore::Event
		{
			static const HashedString DisplayContentsInvalidatedEventType;

			const HashedString & GetEventType() const
			{
				return DisplayContentsInvalidatedEventType;
			}
		};
	}
}