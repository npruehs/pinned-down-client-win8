#include "pch.h"
#include "BitmapResourceHandle.h"
#include "Util\MemoryUtils.h"

using namespace PinnedDownClient::Core;
using namespace PinnedDownClient::Core::Resources;
using namespace PinnedDownClient::Util;

BitmapResourceHandle::BitmapResourceHandle(std::wstring resourceName, ID2D1Bitmap *bitmap)
	: ResourceHandle(resourceName)
{
	this->bitmap = bitmap;
}

BitmapResourceHandle::~BitmapResourceHandle()
{
	SafeRelease(&this->bitmap);
}
