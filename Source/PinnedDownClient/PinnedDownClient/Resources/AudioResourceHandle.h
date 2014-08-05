#pragma once

#include "pch.h"
#include "ResourceHandle.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Resources
	{
		class AudioResourceHandle : public ResourceHandle
		{
		public:
			AudioResourceHandle(std::string resourceName, WAVEFORMATEX* waveFormat, std::shared_ptr<std::vector<BYTE>> audioBuffer);
			~AudioResourceHandle();

			WAVEFORMATEX* waveFormat;
			std::shared_ptr<std::vector<BYTE>> audioBuffer;
		};
	}
}