#pragma once

#include "Core\Event.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct DisplayDpiChangedEvent : public PinnedDownClient::Core::Event
		{
			static const HashedString DisplayDpiChangedEventType;

			const HashedString & GetEventType() const
			{
				return DisplayDpiChangedEventType;
			}

			float logicalDpi;

			explicit DisplayDpiChangedEvent(float logicalDpi)
			{
				this->logicalDpi = logicalDpi;
			}
		};
	}
}