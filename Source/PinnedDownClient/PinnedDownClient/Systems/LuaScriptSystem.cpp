
#include "pch.h"
#include "Event.h"
#include "Systems\LuaScriptSystem.h"

#include "Util\StringUtils.h"

using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Util;

LuaScriptSystem::LuaScriptSystem()
{
}

void LuaScriptSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->InitLua();
}

void LuaScriptSystem::OnEvent(Event & newEvent)
{
}

void LuaScriptSystem::InitLua()
{
	this->lua = std::make_shared<LuaInstance>();
	this->lua->SetLogger(this->game->logger);
}