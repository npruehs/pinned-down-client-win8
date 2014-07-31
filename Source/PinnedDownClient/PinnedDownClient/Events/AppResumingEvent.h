#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct AppResumingEvent : public PinnedDownCore::Event
		{
			static const HashedString AppResumingEventType;

			const HashedString & GetEventType() const
			{
				return AppResumingEventType;
			}
		};
	}
}