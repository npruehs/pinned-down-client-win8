#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct LocalizedTextChangedEvent : public PinnedDownCore::Event
		{
			static const HashedString LocalizedTextChangedEventType;

			const HashedString & GetEventType() const
			{
				return LocalizedTextChangedEventType;
			}

			Entity entity;

			explicit LocalizedTextChangedEvent(Entity entity)
			{
				this->entity = entity;
			}
		};
	}
}