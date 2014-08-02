#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		struct LoginErrorEvent : public PinnedDownCore::Event
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