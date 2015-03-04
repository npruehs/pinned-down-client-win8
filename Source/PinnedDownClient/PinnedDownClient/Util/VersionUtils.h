#pragma once

#include <string>

using namespace Windows::ApplicationModel;

namespace PinnedDownClient
{
	namespace Util
	{
		inline int GetPinnedDownClientVersionBuild()
		{
			auto version = Package::Current->Id->Version;
			return version.Build;
		}

		inline int GetPinnedDownClientVersionMajor()
		{
			auto version = Package::Current->Id->Version;
			return version.Major;
		}

		inline int GetPinnedDownClientVersionMinor()
		{
			auto version = Package::Current->Id->Version;
			return version.Minor;
		}

		inline int GetPinnedDownClientVersionRevision()
		{
			auto version = Package::Current->Id->Version;
			return version.Revision;
		}

		inline std::string GetPinnedDownClientVersionString()
		{
			auto version = Package::Current->Id->Version;
			return "Version " + std::to_string(version.Major) + "." + std::to_string(version.Minor) + "." + std::to_string(version.Build) + "." + std::to_string(version.Revision) + " (Mercury)";
		}
	}
}