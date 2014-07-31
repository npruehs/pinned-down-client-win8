#pragma once

#include "Event.h"
#include "Systems\Screens\ScreenName.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Systems::Screens;

namespace PinnedDownClient
{
	namespace Events
	{
		struct ScreenChangedEvent : public PinnedDownCore::Event
		{
			static const HashedString ScreenChangedEventType;

			const HashedString & GetEventType() const
			{
				return ScreenChangedEventType;
			}

			ScreenName newScreen;

			explicit ScreenChangedEvent(ScreenName newScreen)
			{
				this->newScreen = newScreen;
			}
		};
	}
}