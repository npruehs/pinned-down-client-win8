#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct EntityInitializedEvent : public PinnedDownCore::Event
		{
			static const HashedString EntityInitializedEventType;

			const HashedString & GetEventType() const
			{
				return EntityInitializedEventType;
			}

			Entity entity;

			explicit EntityInitializedEvent(Entity entity)
			{
				this->entity = entity;
			}
		};
	}
}