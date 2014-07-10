#pragma once

#include "Core\Event.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct PointerReleasedEvent : public PinnedDownClient::Core::Event
		{
			static const HashedString PointerReleasedEventType;

			const HashedString & GetEventType() const
			{
				return PointerReleasedEventType;
			}

			uint32 pointerId;
			float positionX;
			float positionY;

			explicit PointerReleasedEvent(uint32 pointerId, float positionX, float positionY)
			{
				this->pointerId = pointerId;
				this->positionX = positionX;
				this->positionY = positionY;
			}
		};
	}
}