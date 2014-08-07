#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct CardCopyCreatedEvent : public PinnedDownCore::Event
		{
			static const HashedString CardCopyCreatedEventType;

			const HashedString & GetEventType() const
			{
				return CardCopyCreatedEventType;
			}

			Entity card;

			explicit CardCopyCreatedEvent(Entity card)
			{
				this->card = card;
			}
		};
	}
}