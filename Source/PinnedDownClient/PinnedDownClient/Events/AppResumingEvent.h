#pragma once

#include "Core\Event.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct AppResumingEvent : public PinnedDownClient::Core::Event
		{
			static const HashedString AppResumingEventType;

			const HashedString & GetEventType() const
			{
				return AppResumingEventType;
			}
		};
	}
}