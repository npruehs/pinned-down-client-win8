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

		inline std::wstring StringToWString(std::string str)
		{
			return std::wstring(str.begin(), str.end());
		}
	}
}