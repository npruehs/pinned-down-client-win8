#pragma once

#include "Event.h"

using namespace Windows::Graphics::Display;

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

			DisplayOrientations orientation;

			explicit DisplayOrientationChangedEvent(DisplayOrientations orientation)
			{
				this->orientation = orientation;
			}

			std::string ToString() const
			{
				std::string orientationString;

				switch (orientation)
				{
				case DisplayOrientations::Landscape:
					orientationString = "Landscape";
					break;
				case DisplayOrientations::LandscapeFlipped:
					orientationString = "LandscapeFlipped";
					break;
				case DisplayOrientations::None:
					orientationString = "None";
					break;
				case DisplayOrientations::Portrait:
					orientationString = "Portrait";
					break;
				case DisplayOrientations::PortraitFlipped:
					orientationString = "PortraitFlipped";
					break;
				}

				return std::string("Display orientation changed: " + orientationString);
			}
		};
	}
}