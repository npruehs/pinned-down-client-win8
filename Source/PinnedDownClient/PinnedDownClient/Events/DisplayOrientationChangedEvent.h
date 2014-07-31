#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct DisplayOrientationChangedEvent : public PinnedDownCore::Event
		{
			static const HashedString DisplayOrientationChangedEventType;

			const HashedString & GetEventType() const
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