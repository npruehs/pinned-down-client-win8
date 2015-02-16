#include "pch.h"
#include "AudioResourceHandle.h"
#include "Util\MemoryUtils.h"

using namespace PinnedDownClient::Resources;
using namespace PinnedDownClient::Util;

AudioResourceHandle::AudioResourceHandle(std::string resourceName, WAVEFORMATEX* waveFormat, std::shared_ptr<std::vector<BYTE>> audioBuffer)
	: ResourceHandle(resourceName),
	audioBuffer(audioBuffer)
{
	this->waveFormat = waveFormat;
}

AudioResourceHandle::~AudioResourceHandle()
{
	CoTaskMemFree(waveFormat);
}
