#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct AppSuspendingEvent : public PinnedDownCore::Event
		{
			static const HashedString AppSuspendingEventType;

			const HashedString & GetEventType() const
			{
				return AppSuspendingEventType;
			}
		};
	}
}