#pragma once

#include "Core\Event.h"

namespace PinnedDownClient
{
	namespace Events
	{
		struct AppResumingEvent : public PinnedDownClient::Core::Event
		{
			static const PinnedDownClient::Util::HashedString AppResumingEventType;

			const PinnedDownClient::Util::HashedString & GetEventType() const
			{
				return AppResumingEventType;
			}
		};
	}
}