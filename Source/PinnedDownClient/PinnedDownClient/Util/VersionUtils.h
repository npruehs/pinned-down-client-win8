#pragma once

#include <string>

using namespace Windows::ApplicationModel;

namespace PinnedDownClient
{
	namespace Util
	{
		inline std::string GetPinnedDownClientVersion()
		{
			auto version = Package::Current->Id->Version;
			return "Version " + std::to_string(version.Major) + "." + std::to_string(version.Minor) + "." + std::to_string(version.Build) + "." + std::to_string(version.Revision) + " (Mercury)";
		}
	}
}