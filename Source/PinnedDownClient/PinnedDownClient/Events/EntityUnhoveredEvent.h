#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct EntityUnhoveredEvent : public PinnedDownCore::Event
		{
			static const HashedString EntityUnhoveredEventType;

			const HashedString & GetEventType() const
			{
				return EntityUnhoveredEventType;
			}

			Entity entity;

			explicit EntityUnhoveredEvent(Entity entity)
			{
				this->entity = entity;
			}

			std::string ToString() const
			{
				return std::string("Entity unhovered: " + std::to_string(this->entity));
			}
		};
	}
}