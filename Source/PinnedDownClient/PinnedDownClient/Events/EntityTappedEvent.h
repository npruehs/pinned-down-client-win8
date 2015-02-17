#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct EntityTappedEvent : public PinnedDownCore::Event
		{
			static const HashedString EntityTappedEventType;

			const HashedString & GetEventType() const
			{
				return EntityTappedEventType;
			}

			Entity entity;

			explicit EntityTappedEvent(Entity entity)
			{
				this->entity = entity;
			}

			std::string ToString() const
			{
				return std::string("Entity tapped: " + std::to_string(this->entity));
			}
		};
	}
}