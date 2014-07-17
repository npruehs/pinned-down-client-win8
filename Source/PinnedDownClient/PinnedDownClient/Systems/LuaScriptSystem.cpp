
#include "pch.h"
#include "Core\Event.h"
#include "Systems\LuaScriptSystem.h"

using namespace PinnedDownClient::Systems;

LuaScriptSystem::LuaScriptSystem()
{
}

void LuaScriptSystem::InitSystem(std::shared_ptr<PinnedDownClient::GameInfrastructure> game)
{
	GameSystem::InitSystem(game);

	this->InitLua();
}

void LuaScriptSystem::OnEvent(Event & newEvent)
{
}

void LuaScriptSystem::InitLua()
{
	int error;
	lua_State *L = luaL_newstate();   /* opens Lua */
	luaL_openlibs(L);

	error = luaL_loadbuffer(L, "i = 3", 5, "line") || lua_pcall(L, 0, 0, 0);

	if (error)
	{
		// Print error message.
		// fprintf(stderr, "%s", lua_tostring(L, -1));

		// Pop error message from the stack.
		lua_pop(L, 1);
	}

	lua_close(L);

	// Next steps: http://www.lua.org/pil/24.2.html
}