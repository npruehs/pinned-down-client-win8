#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct DisplayDpiChangedEvent : public PinnedDownCore::Event
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