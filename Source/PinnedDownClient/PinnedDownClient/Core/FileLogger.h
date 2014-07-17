#pragma once

#include <ppltasks.h>

#include "Util\Logger.h"

using namespace Windows::Storage;
using namespace concurrency;

namespace PinnedDownClient
{
	namespace Core
	{
		class FileLogger : public PinnedDownClient::Util::Logger
		{
		public:
			FileLogger(PinnedDownClient::Util::LogLevel logLevel, const wchar_t* const logFileName);
			~FileLogger();

			void WriteLog(PinnedDownClient::Util::LogLevel logLevel, const wchar_t* const message);
			void Flush();

		private:
			StorageFile^ logFile;
			std::wstring logBuffer;

			std::wstring LogLevelToString(PinnedDownClient::Util::LogLevel logLevel);
		};
	}
}