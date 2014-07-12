#pragma once

#include <string>

namespace PinnedDownClient
{
	namespace Util
	{
		inline std::string WStringToString(std::wstring wstr)
		{
			return std::string(wstr.begin(), wstr.end());
		}
	}
}