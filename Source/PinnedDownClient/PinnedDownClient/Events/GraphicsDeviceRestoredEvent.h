#pragma once

#include "Core\Event.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct GraphicsDeviceRestoredEvent : public PinnedDownClient::Core::Event
		{
			static const HashedString GraphicsDeviceRestoredEventType;

			const HashedString & GetEventType() const
			{
				return GraphicsDeviceRestoredEventType;
			}
		};
	}
}