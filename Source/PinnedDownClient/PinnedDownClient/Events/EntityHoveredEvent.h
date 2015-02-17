#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct EntityHoveredEvent : public PinnedDownCore::Event
		{
			static const HashedString EntityHoveredEventType;

			const HashedString & GetEventType() const
			{
				return EntityHoveredEventType;
			}

			Entity entity;

			explicit EntityHoveredEvent(Entity entity)
			{
				this->entity = entity;
			}

			std::string ToString() const
			{
				return std::string("Entity hovered: " + std::to_string(this->entity));
			}
		};
	}
}