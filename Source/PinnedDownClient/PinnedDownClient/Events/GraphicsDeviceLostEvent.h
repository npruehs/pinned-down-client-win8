#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct GraphicsDeviceLostEvent : public PinnedDownCore::Event
		{
			static const HashedString GraphicsDeviceLostEventType;

			const HashedString & GetEventType() const
			{
				return GraphicsDeviceLostEventType;
			}
		};
	}
}