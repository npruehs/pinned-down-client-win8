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

			std::wstring ToString() const
			{
				std::wstring orientationString;

				switch (orientation)
				{
				case DisplayOrientations::Landscape:
					orientationString = L"Landscape";
					break;
				case DisplayOrientations::LandscapeFlipped:
					orientationString = L"LandscapeFlipped";
					break;
				case DisplayOrientations::None:
					orientationString = L"None";
					break;
				case DisplayOrientations::Portrait:
					orientationString = L"Portrait";
					break;
				case DisplayOrientations::PortraitFlipped:
					orientationString = L"PortraitFlipped";
					break;
				}

				return std::wstring(L"Display orientation changed: " + orientationString);
			}
		};
	}
}