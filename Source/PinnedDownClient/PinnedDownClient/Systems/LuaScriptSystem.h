#pragma once

#include "PinnedDownGame.h"
#include "Core\GameSystem.h"
#include "Core\IEventListener.h"

extern "C" {
#include <Lua\lua.h>
#include <Lua\lauxlib.h>
#include <Lua\lualib.h>
}

using namespace PinnedDownClient::Core;

namespace PinnedDownClient
{
	namespace Systems
	{
		class LuaScriptSystem : public Core::GameSystem, public Core::IEventListener
		{
		public:
			LuaScriptSystem();

			void InitSystem(PinnedDownClient::PinnedDownGame* game);

			void OnEvent(Event & event);

		private:
			void InitLua();
		};
	}
}