#include "pch.h"
#include "Services\MobileServices\PinnedDownMobileService.h"

using namespace PinnedDownClient::Services;

azure::mobile::client& PinnedDownMobileService::GetClient()
{
	
    static azure::mobile::client c(L"https://pinneddown.azure-mobile.net/", 
                                   L"GSzEZQfSDXOyfFdMYpCFlZVZiigAAN83");
    return c;
}
