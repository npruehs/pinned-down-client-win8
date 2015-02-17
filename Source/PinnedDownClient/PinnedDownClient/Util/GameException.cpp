#include "pch.h"
#include "GameException.h"
#include "Util\StringUtils.h"

using namespace PinnedDownClient::Util;

GameException::GameException(const char *msg) : runtime_error(msg)
{
}