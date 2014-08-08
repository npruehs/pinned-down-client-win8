#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct ResourceLoadedEvent : public PinnedDownCore::Event
		{
			static const HashedString ResourceLoadedEventType;

			const HashedString & GetEventType() const
			{
				return ResourceLoadedEventType;
			}

			std::string resourceName;

			explicit ResourceLoadedEvent(std::string resourceName)
			{
				this->resourceName = resourceName;
			}
		};
	}
}