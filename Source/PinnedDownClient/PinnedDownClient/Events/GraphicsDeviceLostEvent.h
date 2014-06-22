#pragma once

#include "Core\Event.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct GraphicsDeviceLostEvent : public PinnedDownClient::Core::Event
		{
			static const HashedString GraphicsDeviceLostEventType;

			const HashedString & GetEventType() const
			{
				return GraphicsDeviceLostEventType;
			}
		};
	}
}