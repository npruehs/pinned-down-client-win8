#pragma once

#include <ppltasks.h>

#include "Logger.h"

using namespace PinnedDownCore;
using namespace Windows::Storage;
using namespace concurrency;

namespace PinnedDownClient
{
	namespace Core
	{
		class FileLogger : public Logger
		{
		public:
			FileLogger(LogLevel logLevel, const wchar_t* const logFileName);
			~FileLogger();

			void WriteLog(LogLevel logLevel, const wchar_t* const message);
			void Flush();

		private:
			StorageFile^ logFile;
			std::wstring logBuffer;
		};
	}
}