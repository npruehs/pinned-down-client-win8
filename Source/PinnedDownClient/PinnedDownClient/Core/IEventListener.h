#pragma once

#include "Event.h"

namespace PinnedDownClient
{
	namespace Core
	{
		class IEventListener
		{
		public:
			virtual void OnEvent(Event const & event) = 0;
		};
	}
}