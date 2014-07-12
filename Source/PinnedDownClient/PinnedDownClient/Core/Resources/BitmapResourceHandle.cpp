#include "pch.h"
#include "BitmapResourceHandle.h"
#include "Util\MemoryUtils.h"

using namespace PinnedDownClient::Core;
using namespace PinnedDownClient::Core::Resources;
using namespace PinnedDownClient::Util;

BitmapResourceHandle::BitmapResourceHandle(char const* const resourceName, ID2D1Bitmap *bitmap)
{
	ResourceHandle::ResourceHandle(resourceName);

	this->bitmap = bitmap;
}

BitmapResourceHandle::~BitmapResourceHandle()
{
	ResourceHandle::~ResourceHandle();

	SafeRelease(&this->bitmap);
}
