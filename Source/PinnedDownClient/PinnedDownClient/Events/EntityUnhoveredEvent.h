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

			std::wstring ToString()
			{
				return std::wstring(L"Entity unhovered: " + std::to_wstring(this->entity));
			}
		};
	}
}