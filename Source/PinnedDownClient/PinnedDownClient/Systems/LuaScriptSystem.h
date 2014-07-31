#pragma once

#include "PinnedDownGame.h"
#include "GameSystem.h"
#include "IEventListener.h"

extern "C" {
#include <Lua\lua.h>
#include <Lua\lauxlib.h>
#include <Lua\lualib.h>
}

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Systems
	{
		class LuaScriptSystem : public GameSystem, public IEventListener
		{
		public:
			LuaScriptSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			void OnEvent(Event & event);

			void InitLua();
		};
	}
}