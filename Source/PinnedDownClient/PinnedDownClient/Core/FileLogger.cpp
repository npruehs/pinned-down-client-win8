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
	SYSTEMTIME st;
	GetSystemTime(&st);
	
	// Format string.
	wchar_t buffer[256];
	swprintf(buffer, 256, L"[%02d-%02d-%d %02d:%02d:%02d] ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	this->logBuffer += std::wstring(buffer) + message;
}

void FileLogger::Flush()
{
	if (this->logFile != nullptr) {
		FileIO::AppendTextAsync(this->logFile, ref new Platform::String(this->logBuffer.c_str()));
	}

	this->logBuffer.clear();
}