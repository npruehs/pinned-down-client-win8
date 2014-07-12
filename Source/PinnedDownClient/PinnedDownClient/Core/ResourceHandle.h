#pragma once

#include "Util\HashedString.h"

using namespace Microsoft::WRL;
using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Core
	{
		class ResourceHandle
		{
		public:
			ResourceHandle();
			ResourceHandle(char const* const resourceName);
			~ResourceHandle();

		private:
			HashedString* resourceName;
		};
	}
}