#pragma once

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

			void InitSystem(std::shared_ptr<Core::EventManager> eventManager, std::shared_ptr<Core::ResourceManager> resourceManager);

			void OnEvent(Event & event);

		private:
			void InitLua();
		};
	}
}