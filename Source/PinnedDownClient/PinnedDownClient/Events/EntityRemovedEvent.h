#pragma once

#include "Core\Event.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct EntityRemovedEvent : public PinnedDownClient::Core::Event
		{
			static const HashedString EntityRemovedEventType;

			const HashedString & GetEventType() const
			{
				return EntityRemovedEventType;
			}

			int entityId;

			explicit EntityRemovedEvent(int entityId)
			{
				this->entityId = entityId;
			}
		};
	}
}