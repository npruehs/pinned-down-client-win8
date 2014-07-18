#pragma once

#include "Core\Event.h"
#include "Math\Vector2F.h"

using namespace PinnedDownClient::Math;
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

			Vector2F size;

			explicit AppWindowSizeChangedEvent(float width, float height)
			{
				this->size = Vector2F(width, height);
			}
		};
	}
}