#pragma once

#include "Util\HashedString.h"
#include <memory>

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

			std::shared_ptr<HashedString> GetResourceName();

		private:
			std::shared_ptr<HashedString> resourceName;
		};
	}
}