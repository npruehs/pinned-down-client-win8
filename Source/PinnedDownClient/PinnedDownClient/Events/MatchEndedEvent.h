#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct MatchEndedEvent : public PinnedDownCore::Event
		{
			static const HashedString MatchEndedEventType;

			const HashedString & GetEventType() const
			{
				return MatchEndedEventType;
			}
		};
	}
}