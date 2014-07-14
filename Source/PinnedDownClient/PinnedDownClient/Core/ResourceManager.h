#pragma once

#include <list>
#include <map>
#include <memory>
#include "ResourceHandle.h"

using namespace Microsoft::WRL;

typedef std::shared_ptr<PinnedDownClient::Core::ResourceHandle> ResHandlePtr;

namespace PinnedDownClient
{
	namespace Core
	{
		class ResourceManager
		{
		public:
			ResourceManager();
			~ResourceManager();

			ResHandlePtr GetResource(HashedString resourceName);
			void UnloadResource(HashedString resourceName);

			template <class T> std::shared_ptr<T> GetResource(HashedString resourceName)
			{
				ResHandlePtr p = this->GetResource(resourceName);
				return std::static_pointer_cast<T>(p);
			}

			// Uses the passed device context to load the image from the specified URI.
			void LoadBitmapFromFile(ID2D1DeviceContext* d2dContext, PCWSTR imageUri);

		private:
			std::map<unsigned long, ResHandlePtr> resourceMap;

			// Windows Imaging Component factory used for loading bitmaps.
			ComPtr<IWICImagingFactory> wicImagingFactory;

			Platform::Array<byte>^ ReadBytes(Platform::String^ fileName);
		};
	}
}