#include "pch.h"
#include "PinnedDownAzure.h"
#include "Services\MobileServices\PinnedDownMobileService.h"

#ifdef REQUIRES_AUTH

using namespace PinnedDownClient::Services;

azure::mobile::client& PinnedDownMobileService::GetClient()
{
	static azure::mobile::client c(L"https://pinneddown.azure-mobile.net/", PINNED_DOWN_APPLICATION_KEY);
    return c;
}

#endif