#pragma once

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
