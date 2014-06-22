#pragma once

#include "Util\HashedString.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Core
	{
		class Event
		{
		public:
			virtual const HashedString & GetEventType() const = 0;
		};
	}
}