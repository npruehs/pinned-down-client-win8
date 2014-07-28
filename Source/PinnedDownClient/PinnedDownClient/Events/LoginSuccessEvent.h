#pragma once

#include "Core\Event.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct LoginSuccessEvent : public PinnedDownClient::Core::Event
		{
			static const HashedString LoginSuccessEventType;

			const HashedString & GetEventType() const
			{
				return LoginSuccessEventType;
			}
		};
	}
}