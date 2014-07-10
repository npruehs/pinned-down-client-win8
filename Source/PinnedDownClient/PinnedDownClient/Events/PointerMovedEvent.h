#pragma once

#include "Core\Event.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct PointerMovedEvent : public PinnedDownClient::Core::Event
		{
			static const HashedString PointerMovedEventType;

			const HashedString & GetEventType() const
			{
				return PointerMovedEventType;
			}

			uint32 pointerId;
			float positionX;
			float positionY;

			explicit PointerMovedEvent(uint32 pointerId, float positionX, float positionY)
			{
				this->pointerId = pointerId;
				this->positionX = positionX;
				this->positionY = positionY;
			}
		};
	}
}