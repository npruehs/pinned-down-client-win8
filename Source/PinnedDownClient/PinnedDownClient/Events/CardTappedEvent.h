#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct CardTappedEvent : public PinnedDownCore::Event
		{
			static const HashedString CardTappedEventType;

			const HashedString & GetEventType() const
			{
				return CardTappedEventType;
			}

			Entity entity;

			explicit CardTappedEvent(Entity entity)
			{
				this->entity = entity;
			}

			std::string ToString() const
			{
				return std::string("Card tapped: " + std::to_string(this->entity));
			}
		};
	}
}