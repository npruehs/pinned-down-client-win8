#pragma once

#include "ResourceHandle.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Resources
	{
		class BitmapResourceHandle : public ResourceHandle
		{
		public:
			BitmapResourceHandle(std::wstring resourceName, ID2D1Bitmap *bitmap);
			~BitmapResourceHandle();

			ID2D1Bitmap *bitmap;
		};
	}
}