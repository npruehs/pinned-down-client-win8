#pragma once

#include "Core\Event.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct AppSuspendingEvent : public PinnedDownClient::Core::Event
		{
			static const HashedString AppSuspendingEventType;

			const HashedString & GetEventType() const
			{
				return AppSuspendingEventType;
			}
		};
	}
}