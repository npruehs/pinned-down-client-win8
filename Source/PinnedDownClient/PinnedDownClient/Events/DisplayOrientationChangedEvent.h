#pragma once

#include "Core\Event.h"

namespace PinnedDownClient
{
	namespace Events
	{
		struct DisplayOrientationChangedEvent : public PinnedDownClient::Core::Event
		{
			static const PinnedDownClient::Util::HashedString DisplayOrientationChangedEventType;

			const PinnedDownClient::Util::HashedString & GetEventType() const
			{
				return DisplayOrientationChangedEventType;
			}

			Windows::Graphics::Display::DisplayOrientations orientation;

			explicit DisplayOrientationChangedEvent(Windows::Graphics::Display::DisplayOrientations orientation)
			{
				this->orientation = orientation;
			}
		};
	}
}