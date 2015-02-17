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

			std::string errorMessage;

			explicit LoginErrorEvent(std::string errorMessage)
			{
				this->errorMessage = errorMessage;
			}

			std::string ToString() const
			{
				return std::string("Login error " + this->errorMessage);
			}
		};
	}
}