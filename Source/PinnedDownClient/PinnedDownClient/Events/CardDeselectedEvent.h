#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct CardDeselectedEvent : public PinnedDownCore::Event
		{
			static const HashedString CardDeselectedEventType;

			const HashedString & GetEventType() const
			{
				return CardDeselectedEventType;
			}

			Entity entity;

			explicit CardDeselectedEvent(Entity entity)
			{
				this->entity = entity;
			}
		};
	}
}