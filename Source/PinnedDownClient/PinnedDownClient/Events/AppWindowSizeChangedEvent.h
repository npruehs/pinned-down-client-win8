#pragma once

#include "Core\Event.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct AppWindowSizeChangedEvent : public PinnedDownClient::Core::Event
		{
			static const HashedString AppWindowSizeChangedEventType;

			const HashedString & GetEventType() const
			{
				return AppWindowSizeChangedEventType;
			}

			float width;
			float height;

			explicit AppWindowSizeChangedEvent(float width, float height)
			{
				this->width = width;
				this->height = height;
			}
		};
	}
}