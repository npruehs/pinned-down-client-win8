#pragma once

#include "EntityManager.h"
#include "Event.h"
#include "Math\Vector2F.h"

using namespace PinnedDownClient::Math;
using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct PositionChangedEvent : public PinnedDownCore::Event
		{
			static const HashedString PositionChangedEventType;

			const HashedString & GetEventType() const
			{
				return PositionChangedEventType;
			}

			Entity entity;
			Vector2F position;

			explicit PositionChangedEvent(Entity entity, Vector2F position)
			{
				this->entity = entity;
				this->position = position;
			}

			std::string ToString() const
			{
				return std::string("Entity " + std::to_string(entity) + " position changed: " + position.ToString());
			}
		};
	}
}