#pragma once

#include "pch.h"
#include "Core\ResourceHandle.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Core
	{
		namespace Resources
		{
			class AudioResourceHandle : public PinnedDownClient::Core::ResourceHandle
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