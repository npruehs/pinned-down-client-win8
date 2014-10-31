#pragma once

#include "EntityManager.h"
#include "Event.h"

#include "Systems\UI\UIMode.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct UIModeChangedEvent : public PinnedDownCore::Event
		{
			static const HashedString UIModeChangedEventType;

			const HashedString & GetEventType() const
			{
				return UIModeChangedEventType;
			}

			Systems::UI::UIMode newMode;

			explicit UIModeChangedEvent(Systems::UI::UIMode newMode)
			{
				this->newMode = newMode;
			}
		};
	}
}