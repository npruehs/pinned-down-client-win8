#pragma once

#include "Core\Event.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct PointerPressedEvent : public PinnedDownClient::Core::Event
		{
			static const HashedString PointerPressedEventType;

			const HashedString & GetEventType() const
			{
				return PointerPressedEventType;
			}

			uint32 pointerId;
			float positionX;
			float positionY;

			explicit PointerPressedEvent(uint32 pointerId, float positionX, float positionY)
			{
				this->pointerId = pointerId;
				this->positionX = positionX;
				this->positionY = positionY;
			}
		};
	}
}