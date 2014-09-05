#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct DisconnectedFromServerEvent : public PinnedDownCore::Event
		{
			static const HashedString DisconnectedFromServerEventType;

			const HashedString & GetEventType() const
			{
				return DisconnectedFromServerEventType;
			}
		};
	}
}