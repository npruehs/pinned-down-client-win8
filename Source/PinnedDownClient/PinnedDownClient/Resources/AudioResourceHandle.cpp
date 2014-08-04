#include "pch.h"
#include "AudioResourceHandle.h"
#include "Util\MemoryUtils.h"

using namespace PinnedDownClient::Resources;
using namespace PinnedDownClient::Util;

AudioResourceHandle::AudioResourceHandle(std::wstring resourceName, WAVEFORMATEX* waveFormat, std::shared_ptr<std::vector<BYTE>> audioBuffer)
	: ResourceHandle(resourceName)
{
	this->waveFormat = waveFormat;
	this->audioBuffer = audioBuffer;
}

AudioResourceHandle::~AudioResourceHandle()
{
	CoTaskMemFree(waveFormat);
}
