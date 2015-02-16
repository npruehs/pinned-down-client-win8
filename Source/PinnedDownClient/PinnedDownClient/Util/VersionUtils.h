#pragma once

#include <string>

using namespace Windows::ApplicationModel;

namespace PinnedDownClient
{
	namespace Util
	{
		inline std::wstring GetPinnedDownClientVersion()
		{
			auto version = Package::Current->Id->Version;
			return L"Version " + std::to_wstring(version.Major) + L"." + std::to_wstring(version.Minor) + L"." + std::to_wstring(version.Build) + L"." + std::to_wstring(version.Revision) + L" (Mercury)";
		}
	}
}