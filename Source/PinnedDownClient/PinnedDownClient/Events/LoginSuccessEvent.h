#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct LoginSuccessEvent : public PinnedDownCore::Event
		{
			static const HashedString LoginSuccessEventType;

			const HashedString & GetEventType() const
			{
				return LoginSuccessEventType;
			}
		};
	}
}