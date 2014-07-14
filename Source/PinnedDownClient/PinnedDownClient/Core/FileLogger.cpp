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
	this->logBuffer += message;
}

void FileLogger::Flush()
{
	if (this->logFile != nullptr) {
		FileIO::AppendTextAsync(this->logFile, ref new Platform::String(this->logBuffer.c_str()));
	}

	this->logBuffer.clear();
}