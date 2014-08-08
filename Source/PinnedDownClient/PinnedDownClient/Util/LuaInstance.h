#pragma once

extern "C" {
#include <Lua\lua.h>
#include <Lua\lauxlib.h>
#include <Lua\lualib.h>
}

#include "Logger.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Util
	{
		class LuaInstance
		{
		public:
			LuaInstance();
			~LuaInstance();

			bool ExecuteLine(const char* line);

			void SetLogger(std::shared_ptr<Logger> logger);

			bool GetBool(const char* var);
			int GetInt(const char* var);
			float GetFloat(const char* var);
			std::string GetString(const char* var);

			bool GetBool(const char* table, const char* key);
			int GetInt(const char* table, const char* key);
			float GetFloat(const char* table, const char* key);
			std::string GetString(const char* table, const char* key);

		private:
			int error;
			lua_State* luaState;
			std::shared_ptr<Logger> logger;

			void LogError(const char* message);

			bool PushTableValue(const char* table, const char* key);

			bool PopBool(const char* var);
			int PopInt(const char* var);
			float PopFloat(const char* var);
			std::string PopString(const char* var);
		};
	}
}