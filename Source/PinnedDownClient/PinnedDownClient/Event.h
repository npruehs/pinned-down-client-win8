#pragma once

#include "Util\HashedString.h"

namespace PinnedDownClient
{
	namespace Core
	{
		class Event
		{
		public:
			virtual const PinnedDownClient::Util::HashedString & GetEventType() const = 0;
		};
	}
}