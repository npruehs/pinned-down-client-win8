#pragma once

#include "Core\Event.h"

namespace PinnedDownClient
{
	namespace Events
	{
		struct AppSuspendingEvent : public PinnedDownClient::Core::Event
		{
			static const PinnedDownClient::Util::HashedString AppSuspendingEventType;

			const PinnedDownClient::Util::HashedString & GetEventType() const
			{
				return AppSuspendingEventType;
			}
		};
	}
}