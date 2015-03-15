#pragma once

#include "Event.h"
#include "Math\Vector2F.h"

using namespace PinnedDownClient::Math;
using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct PointerDraggedEvent : public PinnedDownCore::Event
		{
			static const HashedString PointerDraggedEventType;

			const HashedString & GetEventType() const
			{
				return PointerDraggedEventType;
			}

			Vector2F delta;

			explicit PointerDraggedEvent(Vector2F delta)
			{
				this->delta = delta;
			}

			std::string ToString() const
			{
				return std::string("Pointer dragged: " + delta.ToString());
			}
		};
	}
}