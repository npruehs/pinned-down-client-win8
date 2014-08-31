#pragma once

#include "Event.h"

#include "Util\ClientIdMapping.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct ClientIdMappingCreatedEvent : public PinnedDownCore::Event
		{
			static const HashedString ClientIdMappingCreatedEventType;

			const HashedString & GetEventType() const
			{
				return ClientIdMappingCreatedEventType;
			}

			std::shared_ptr<ClientIdMapping> clientIdMapping;

			explicit ClientIdMappingCreatedEvent(std::shared_ptr<ClientIdMapping> clientIdMapping)
			{
				this->clientIdMapping = clientIdMapping;
			}
		};
	}
}