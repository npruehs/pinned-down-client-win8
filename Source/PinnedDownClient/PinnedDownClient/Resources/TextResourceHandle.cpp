#include "pch.h"
#include "TextResourceHandle.h"

using namespace PinnedDownClient::Resources;


TextResourceHandle::TextResourceHandle(std::string resourceName, std::wstring text)
	: ResourceHandle(resourceName)
{
	this->text = text;
}
