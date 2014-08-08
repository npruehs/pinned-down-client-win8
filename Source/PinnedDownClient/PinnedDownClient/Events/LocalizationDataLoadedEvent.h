#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct LocalizationDataLoadedEvent : public PinnedDownCore::Event
		{
			static const HashedString LocalizationDataLoadedEventType;

			const HashedString & GetEventType() const
			{
				return LocalizationDataLoadedEventType;
			}
		};
	}
}