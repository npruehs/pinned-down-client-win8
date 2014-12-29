#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct CardSelectedEvent : public PinnedDownCore::Event
		{
			static const HashedString CardSelectedEventType;

			const HashedString & GetEventType() const
			{
				return CardSelectedEventType;
			}

			Entity entity;

			explicit CardSelectedEvent(Entity entity)
			{
				this->entity = entity;
			}

			std::wstring ToString()
			{
				return std::wstring(L"Card selected: " + std::to_wstring(this->entity));
			}
		};
	}
}