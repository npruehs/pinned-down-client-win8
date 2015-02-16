#include "pch.h"
#include "BitmapResourceHandle.h"
#include "Util\MemoryUtils.h"

using namespace PinnedDownClient::Resources;
using namespace PinnedDownClient::Util;

BitmapResourceHandle::BitmapResourceHandle(std::string resourceName, ID2D1Bitmap *bitmap)
	: ResourceHandle(resourceName)
{
	this->bitmap = bitmap;
}

BitmapResourceHandle::~BitmapResourceHandle()
{
	SafeRelease(&this->bitmap);
}
