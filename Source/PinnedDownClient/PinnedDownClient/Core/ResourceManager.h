#pragma once

#include <list>
#include <map>
#include <memory>
#include "pch.h"
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

			ResHandlePtr GetResource(HashedString resourceName);

			template <class T> T& GetResource(HashedString resourceName)
			{
				ResHandlePtr p = this->GetResource(resourceName);
				ResourceHandle & resourceHandle = *p;
				T & handle = static_cast<T&>(resourceHandle);
				return handle;
			}

			// Uses the passed device context to load the image from the specified URI.
			void LoadBitmapFromFile(ID2D1DeviceContext* d2dContext, const char* resourceUri, PCWSTR imageUri);

		private:
			std::list<ResHandlePtr> resourceList;
			std::map<unsigned long, ResHandlePtr> resourceMap;

			// Windows Imaging Component factory used for loading bitmaps.
			ComPtr<IWICImagingFactory> wicImagingFactory;
		};
	}
}