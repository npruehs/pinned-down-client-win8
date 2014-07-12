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
	this->resourceName = std::make_shared<HashedString>(resourceName);
}

std::shared_ptr<HashedString> ResourceHandle::GetResourceName()
{
	return std::shared_ptr<HashedString>(this->resourceName);
}

ResourceHandle::~ResourceHandle()
{
}