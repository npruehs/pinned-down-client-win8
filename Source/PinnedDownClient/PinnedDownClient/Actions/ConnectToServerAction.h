#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct ConnectToServerAction : public PinnedDownCore::Event
		{
			static const HashedString ConnectToServerActionType;

			const HashedString & GetEventType() const
			{
				return ConnectToServerActionType;
			}
		};
	}
}