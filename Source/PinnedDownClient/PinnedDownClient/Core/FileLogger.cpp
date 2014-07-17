#include "pch.h"
#include "FileLogger.h"

using namespace PinnedDownClient::Core;
using namespace PinnedDownClient::Util;

FileLogger::FileLogger(LogLevel logLevel, const wchar_t* const logFileName) : Logger(logLevel)
{
	StorageFolder^ temporaryFolder = ApplicationData::Current->TemporaryFolder;

	create_task(temporaryFolder->CreateFileAsync(ref new Platform::String(logFileName), CreationCollisionOption::ReplaceExisting))
		.then([this](StorageFile^ logFile) {
		this->logFile = logFile;
	});
}

FileLogger::~FileLogger()
{
	this->Flush();
}

void FileLogger::WriteLog(LogLevel logLevel, const wchar_t* const message)
{
	// Get current time.
	SYSTEMTIME systemTime;
	GetSystemTime(&systemTime);

	// Format string.
	wchar_t systemTimeBuffer[256];
	swprintf
		(systemTimeBuffer,
		 256,
		 L"[%02d-%02d-%d %02d:%02d:%02d.%03d] ",
		 systemTime.wYear,
		 systemTime.wMonth,
		 systemTime.wDay,
		 systemTime.wHour,
		 systemTime.wMinute,
		 systemTime.wSecond,
		 systemTime.wMilliseconds);

	this->logBuffer += std::wstring(systemTimeBuffer) + this->LogLevelToString(logLevel) + L" - " + message;
}

void FileLogger::Flush()
{
	if (this->logFile != nullptr) {
		FileIO::AppendTextAsync(this->logFile, ref new Platform::String(this->logBuffer.c_str()));
	}

	this->logBuffer.clear();
}

std::wstring FileLogger::LogLevelToString(PinnedDownClient::Util::LogLevel logLevel)
{
	switch (logLevel)
	{
	case LogLevel::Debug:
		return L"DEBUG";
	case LogLevel::Error:
		return L"ERROR";
	case LogLevel::Fatal:
		return L"FATAL";
	case LogLevel::Info:
		return L"INFO";
	case LogLevel::Trace:
		return L"TRACE";
	case LogLevel::Warn:
		return L"WARN";
	default:
		return L"LOG";
	}
}