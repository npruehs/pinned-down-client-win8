#pragma once

//#define REQUIRES_AUTH

#ifdef REQUIRES_AUTH

#include <azuremobile.h>

namespace PinnedDownClient
{
	namespace Services
	{
		class PinnedDownMobileService
		{
		public:
			static azure::mobile::client& GetClient();
		};
	}
}

#endif