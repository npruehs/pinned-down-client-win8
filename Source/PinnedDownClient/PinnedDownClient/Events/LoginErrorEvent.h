#pragma once

#include "Core\Event.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		struct LoginErrorEvent : public PinnedDownClient::Core::Event
		{
			static const HashedString LoginErrorEventType;

			const HashedString & GetEventType() const
			{
				return LoginErrorEventType;
			}

			std::wstring errorMessage;

			explicit LoginErrorEvent(std::wstring errorMessage)
			{
				this->errorMessage = errorMessage;
			}
		};
	}
}