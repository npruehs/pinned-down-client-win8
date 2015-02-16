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

			std::wstring ToString() const
			{
				return std::wstring(L"Entity hovered: " + std::to_wstring(this->entity));
			}
		};
	}
}