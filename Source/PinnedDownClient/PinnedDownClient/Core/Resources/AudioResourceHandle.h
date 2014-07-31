#pragma once

#include "pch.h"
#include "ResourceHandle.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Core
	{
		namespace Resources
		{
			class AudioResourceHandle : public ResourceHandle
			{
			public:
				AudioResourceHandle(std::wstring resourceName, WAVEFORMATEX* waveFormat, std::vector<BYTE>* audioBuffer);
				~AudioResourceHandle();

				WAVEFORMATEX* waveFormat;
				std::vector<BYTE>* audioBuffer;
			};
		}
	}
}