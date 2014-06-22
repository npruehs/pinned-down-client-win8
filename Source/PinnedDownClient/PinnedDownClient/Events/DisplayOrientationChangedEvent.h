#pragma once

#include "Core\Event.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct DisplayOrientationChangedEvent : public PinnedDownClient::Core::Event
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