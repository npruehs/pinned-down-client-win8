#pragma once

#include "Core\Event.h"
#include "Math\Vector2F.h"

using namespace PinnedDownClient::Math;
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
			Vector2F position;

			explicit PointerPressedEvent(uint32 pointerId, float positionX, float positionY)
			{
				this->pointerId = pointerId;
				this->position = Vector2F(positionX, positionY);
			}
		};
	}
}