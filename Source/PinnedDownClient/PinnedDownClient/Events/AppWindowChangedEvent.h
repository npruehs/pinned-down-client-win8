#pragma once

#include "Core\Event.h"

namespace PinnedDownClient
{
	namespace Events
	{
		struct AppWindowChangedEvent : public PinnedDownClient::Core::Event
		{
			static const PinnedDownClient::Util::HashedString AppWindowChangedEventType;

			const PinnedDownClient::Util::HashedString & GetEventType() const
			{
				return AppWindowChangedEventType;
			}

			Windows::UI::Core::CoreWindow^ appWindow;

			explicit AppWindowChangedEvent(Windows::UI::Core::CoreWindow^ appWindow)
			{
				this->appWindow = appWindow;
			}
		};
	}
}