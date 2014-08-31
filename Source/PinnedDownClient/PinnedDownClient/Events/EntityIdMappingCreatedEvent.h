#pragma once

#include "Event.h"

#include "Util\EntityIdMapping.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct EntityIdMappingCreatedEvent : public PinnedDownCore::Event
		{
			static const HashedString EntityIdMappingCreatedEventType;

			const HashedString & GetEventType() const
			{
				return EntityIdMappingCreatedEventType;
			}

			std::shared_ptr<EntityIdMapping> entityIdMapping;

			explicit EntityIdMappingCreatedEvent(std::shared_ptr<EntityIdMapping> entityIdMapping)
			{
				this->entityIdMapping = entityIdMapping;
			}
		};
	}
}