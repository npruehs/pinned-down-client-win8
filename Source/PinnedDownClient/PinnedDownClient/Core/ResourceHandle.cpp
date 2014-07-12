#include "pch.h"
#include "ResourceHandle.h"
#include "Util\MemoryUtils.h"

using namespace PinnedDownClient::Core;
using namespace PinnedDownClient::Util;

ResourceHandle::ResourceHandle()
{
}

ResourceHandle::ResourceHandle(char const* const resourceName)
{
	this->resourceName = new HashedString(resourceName);
}

ResourceHandle::~ResourceHandle()
{
	delete this->resourceName;
}