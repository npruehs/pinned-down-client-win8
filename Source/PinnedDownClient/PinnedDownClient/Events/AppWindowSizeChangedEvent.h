#pragma once

#include "Core\Event.h"

namespace PinnedDownClient
{
	namespace Events
	{
		struct AppWindowSizeChangedEvent : public PinnedDownClient::Core::Event
		{
			static const PinnedDownClient::Util::HashedString AppWindowSizeChangedEventType;

			const PinnedDownClient::Util::HashedString & GetEventType() const
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