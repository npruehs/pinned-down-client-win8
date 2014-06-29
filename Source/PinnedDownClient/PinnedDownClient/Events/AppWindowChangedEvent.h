#pragma once

#include "Core\Event.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct AppWindowChangedEvent : public PinnedDownClient::Core::Event
		{
			static const HashedString AppWindowChangedEventType;

			const HashedString & GetEventType() const
			{
				return AppWindowChangedEventType;
			}

			Platform::Agile<Windows::UI::Core::CoreWindow> appWindow;

			explicit AppWindowChangedEvent(Platform::Agile<Windows::UI::Core::CoreWindow> appWindow)
			{
				this->appWindow = appWindow;
			}
		};
	}
}