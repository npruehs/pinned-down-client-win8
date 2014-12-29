#pragma once

#include "Event.h"
#include "Math\Vector2F.h"

using namespace PinnedDownClient::Math;
using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct PointerReleasedEvent : public PinnedDownCore::Event
		{
			static const HashedString PointerReleasedEventType;

			const HashedString & GetEventType() const
			{
				return PointerReleasedEventType;
			}

			uint32 pointerId;
			Vector2F position;

			explicit PointerReleasedEvent(uint32 pointerId, float positionX, float positionY)
			{
				this->pointerId = pointerId;
				this->position = Vector2F(positionX, positionY);
			}

			std::wstring ToString()
			{
				return std::wstring(L"Pointer " + std::to_wstring(pointerId) + L" released: " + position.ToString());
			}
		};
	}
}