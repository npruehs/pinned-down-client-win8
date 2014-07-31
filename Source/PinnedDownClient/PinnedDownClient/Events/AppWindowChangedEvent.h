#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct AppWindowChangedEvent : public PinnedDownCore::Event
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