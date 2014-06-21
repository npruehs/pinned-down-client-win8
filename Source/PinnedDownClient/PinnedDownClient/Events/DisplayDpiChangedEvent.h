#pragma once

#include "Core\Event.h"

namespace PinnedDownClient
{
	namespace Events
	{
		struct DisplayDpiChangedEvent : public PinnedDownClient::Core::Event
		{
			static const PinnedDownClient::Util::HashedString DisplayDpiChangedEventType;

			const PinnedDownClient::Util::HashedString & GetEventType() const
			{
				return DisplayDpiChangedEventType;
			}

			float dpi;

			explicit DisplayDpiChangedEvent(float dpi)
			{
				this->dpi = dpi;
			}
		};
	}
}