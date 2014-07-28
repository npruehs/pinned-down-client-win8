#pragma once

#include "Core\Event.h"
#include "Systems\Screens\ScreenName.h"

using namespace PinnedDownClient::Systems::Screens;

namespace PinnedDownClient
{
	namespace Events
	{
		struct ScreenChangedEvent : public PinnedDownClient::Core::Event
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