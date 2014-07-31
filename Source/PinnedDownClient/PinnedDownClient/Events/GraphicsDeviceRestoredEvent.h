#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct GraphicsDeviceRestoredEvent : public PinnedDownCore::Event
		{
			static const HashedString GraphicsDeviceRestoredEventType;

			const HashedString & GetEventType() const
			{
				return GraphicsDeviceRestoredEventType;
			}
		};
	}
}