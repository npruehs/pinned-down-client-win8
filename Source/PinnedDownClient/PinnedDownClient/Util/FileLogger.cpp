#include "pch.h"
#include "FileLogger.h"

#include "Util\StringUtils.h"

using namespace PinnedDownClient::Util;

FileLogger::FileLogger(LogLevel logLevel, const char* const logFileName) : Logger(logLevel)
{
	StorageFolder^ temporaryFolder = ApplicationData::Current->TemporaryFolder;

	auto logFileNameWide = StringToWString(logFileName);

	create_task(temporaryFolder->CreateFileAsync(ref new Platform::String(logFileNameWide.c_str()), CreationCollisionOption::ReplaceExisting))
		.then([this](StorageFile^ logFile) {
		this->logFile = logFile;
	});
}

FileLogger::~FileLogger()
{
	this->Flush();
}

void FileLogger::WriteLog(LogLevel logLevel, const char* const message)
{
	// Get current time.
	SYSTEMTIME systemTime;
	GetSystemTime(&systemTime);

	// Format string.
	char systemTimeBuffer[256];
	sprintf_s
		(systemTimeBuffer,
		 "[%02d-%02d-%d %02d:%02d:%02d.%03d] ",
		 systemTime.wYear,
		 systemTime.wMonth,
		 systemTime.wDay,
		 systemTime.wHour,
		 systemTime.wMinute,
		 systemTime.wSecond,
		 systemTime.wMilliseconds);

	this->logBuffer += std::string(systemTimeBuffer) + this->LogLevelToString(logLevel) + " - " + message;
}

void FileLogger::Flush()
{
	if (this->logFile != nullptr) {
		auto bufferedLogWide = StringToWString(this->logBuffer);
		FileIO::AppendTextAsync(this->logFile, ref new Platform::String(bufferedLogWide.c_str()));
	}

	this->logBuffer.clear();
}
