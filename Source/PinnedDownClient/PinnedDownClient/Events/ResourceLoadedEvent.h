#pragma once

#include "Event.h"
#include "Util\StringUtils.h"

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

			std::wstring ToString()
			{
				return std::wstring(L"Resource loaded: " + PinnedDownClient::Util::StringToWString(resourceName));
			}
		};
	}
}