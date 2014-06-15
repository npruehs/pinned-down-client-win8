#pragma once

#include "Event.h"

namespace PinnedDownClient
{
	namespace Core
	{
		class IEventListener
		{
		public:
			virtual void OnEvent(Event & event) = 0;
		};
	}
}