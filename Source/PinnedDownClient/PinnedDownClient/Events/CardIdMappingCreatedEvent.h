#pragma once

#include "Event.h"

#include "Util\CardIdMapping.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct CardIdMappingCreatedEvent : public PinnedDownCore::Event
		{
			static const HashedString CardIdMappingCreatedEventType;

			const HashedString & GetEventType() const
			{
				return CardIdMappingCreatedEventType;
			}

			std::shared_ptr<CardIdMapping> cardIdMapping;

			explicit CardIdMappingCreatedEvent(std::shared_ptr<CardIdMapping> cardIdMapping)
			{
				this->cardIdMapping = cardIdMapping;
			}
		};
	}
}