#pragma once

#include <ppltasks.h>

#include "Logger.h"

using namespace PinnedDownCore;
using namespace Windows::Storage;
using namespace concurrency;

namespace PinnedDownClient
{
	namespace Util
	{
		class FileLogger : public Logger
		{
		public:
			FileLogger(LogLevel logLevel, const char* const logFileName);
			~FileLogger();

			void WriteLog(LogLevel logLevel, const char* const message);
			void Flush();

		private:
			StorageFile^ logFile;
			std::string logBuffer;
		};
	}
}