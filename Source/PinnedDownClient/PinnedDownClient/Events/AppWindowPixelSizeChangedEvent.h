#pragma once

#include "Event.h"

#include "Math\Vector2F.h"

using namespace PinnedDownClient::Math;
using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct AppWindowPixelSizeChangedEvent : public PinnedDownCore::Event
		{
			static const HashedString AppWindowPixelSizeChangedEventType;

			const HashedString & GetEventType() const
			{
				return AppWindowPixelSizeChangedEventType;
			}

			const Vector2F pixelSize;

			explicit AppWindowPixelSizeChangedEvent(Vector2F pixelSize)
				: pixelSize(pixelSize)
			{
			}

			explicit AppWindowPixelSizeChangedEvent(float width, float height)
				: pixelSize(Vector2F(width, height))
			{
			}

			std::string ToString() const
			{
				return std::string("App window pixel size changed: " + this->pixelSize.ToString());
			}
		};
	}
}