#pragma once

#include "Core\ResourceHandle.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Core
	{
		namespace Resources
		{
			class BitmapResourceHandle : public PinnedDownClient::Core::ResourceHandle
			{
			public:
				BitmapResourceHandle(char const* const resourceName, ID2D1Bitmap *bitmap);
				~BitmapResourceHandle();

				ID2D1Bitmap *bitmap;
			};
		}
	}
}