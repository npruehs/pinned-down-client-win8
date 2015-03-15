#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct CardCameraCreatedEvent : public PinnedDownCore::Event
		{
			static const HashedString CardCameraCreatedEventType;

			const HashedString & GetEventType() const
			{
				return CardCameraCreatedEventType;
			}

			Entity entity;

			explicit CardCameraCreatedEvent(Entity entity)
			{
				this->entity = entity;
			}

			std::string ToString() const
			{
				return std::string("Card camera created: " + std::to_string(this->entity));
			}
		};
	}
}