#pragma once

#include <string>
#include "ResourceHandle.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Resources
	{
		class TextResourceHandle : public ResourceHandle
		{
		public:
			TextResourceHandle(std::string resourceName, std::wstring text);

			std::wstring text;
		};
	}
}