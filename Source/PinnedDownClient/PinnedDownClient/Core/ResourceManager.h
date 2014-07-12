#pragma once

#include "pch.h"

using namespace Microsoft::WRL;

namespace PinnedDownClient
{
	namespace Core
	{
		class ResourceManager
		{
		public:
			ResourceManager();

			// Uses the passed device context to load the image from the specified URI.
			void LoadBitmapFromFile(ID2D1DeviceContext* d2dContext, PCWSTR imageUri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap **bitmap);

		private:
			// Windows Imaging Component factory used for loading bitmaps.
			ComPtr<IWICImagingFactory> wicImagingFactory;
		};
	}
}