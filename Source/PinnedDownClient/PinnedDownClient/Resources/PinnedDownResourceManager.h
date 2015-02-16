#pragma once

#include "pch.h"
#include "Game.h"
#include "ResourceManager.h"

using namespace PinnedDownCore;
using namespace Microsoft::WRL;

namespace PinnedDownClient
{
	namespace Resources
	{
		class PinnedDownResourceManager : public ResourceManager
		{
		public:
			PinnedDownResourceManager(PinnedDownCore::Game* game);

			// Uses the passed device context to load the image from the specified URI.
			void LoadBitmapFromFile(ID2D1DeviceContext* d2dContext, PCWSTR imageUri);

			void LoadAudioFromFile(IXAudio2* engine, LPCWSTR audioUri);
			void LoadTextFromFile(std::wstring fileUri);

		private:
			// Windows Imaging Component factory used for loading bitmaps.
			ComPtr<IWICImagingFactory> wicImagingFactory;

			Platform::Array<byte>^ ReadBytes(Platform::String^ fileName);
		};
	}
}