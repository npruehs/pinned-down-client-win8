
#include "pch.h"

#include "Util\LuaInstance.h"
#include "Util\StringUtils.h"


using namespace PinnedDownClient::Util;


LuaInstance::LuaInstance()
{
	this->luaState = luaL_newstate();
	luaL_openlibs(this->luaState);
}

LuaInstance::~LuaInstance()
{
	lua_close(this->luaState);
}

void LuaInstance::SetLogger(std::shared_ptr<Logger> logger)
{
	this->logger = logger;
}

bool LuaInstance::ExecuteScript(const char* script)
{
	error = luaL_loadbuffer(this->luaState, script, strlen(script), "script") || lua_pcall(this->luaState, 0, 0, 0);

	// Check for errors.
	if (error)
	{
		// Log error.
		const char* errorMessage = lua_tostring(this->luaState, -1);
		this->LogError(errorMessage);

		// Pop error message from the stack.
		lua_pop(this->luaState, 1);
	}

	return !error;
}

bool LuaInstance::GetBool(const char* var)
{
	lua_getglobal(this->luaState, var);
	return this->PopBool(var);
}

bool LuaInstance::GetBool(const char* table, const char* key)
{
	if (this->PushTableValue(table, key))
	{
		return this->PopBool(key);
	}

	return false;
}

int LuaInstance::GetInt(const char* var)
{
	lua_getglobal(this->luaState, var);
	return this->PopInt(var);
}

int LuaInstance::GetInt(const char* table, const char* key)
{
	if (this->PushTableValue(table, key))
	{
		return this->PopInt(key);
	}

	return 0;
}

float LuaInstance::GetFloat(const char* var)
{
	lua_getglobal(this->luaState, var);
	return this->PopFloat(var);
}

float LuaInstance::GetFloat(const char* table, const char* key)
{
	if (this->PushTableValue(table, key))
	{
		return this->PopFloat(key);
	}

	return 0.0f;
}

std::string LuaInstance::GetString(const char* var)
{
	lua_getglobal(this->luaState, var);
	return this->PopString(var);
}

std::string LuaInstance::GetString(const char* table, const char* key)
{
	if (this->PushTableValue(table, key))
	{
		return this->PopString(key);
	}

	return "";
}

bool LuaInstance::PushTableValue(const char* table, const char* key)
{
	// Push table on stack.
	lua_getglobal(this->luaState, table);

	if (!lua_istable(this->luaState, -1))
	{
		// Log error.
		std::string errorMessage = table;
		errorMessage += " is not a table.";
		this->LogError(errorMessage.c_str());
		lua_pop(this->luaState, 1);
		return false;
	}
	else
	{
		// Push table value on stack.
		lua_getfield(this->luaState, -1, key);
		return true;
	}
}

bool LuaInstance::PopBool(const char* var)
{
	if (!lua_isboolean(this->luaState, -1))
	{
		// Log error.
		std::string errorMessage = var;
		errorMessage += " is not a boolean value.";
		this->LogError(errorMessage.c_str());
		lua_pop(this->luaState, 1);
		return false;
	}
	else
	{
		// Convert and return value.
		auto result = lua_toboolean(this->luaState, -1) != 0;
		lua_pop(this->luaState, 1);
		return result;
	}
}

int LuaInstance::PopInt(const char* var)
{
	if (!lua_isnumber(this->luaState, -1))
	{
		// Log error.
		std::string errorMessage = var;
		errorMessage += " is not a number.";
		this->LogError(errorMessage.c_str());
		lua_pop(this->luaState, 1);
		return false;
	}
	else
	{
		// Convert and return value.
		auto result = (int)lua_tonumber(this->luaState, -1);
		lua_pop(this->luaState, 1);
		return result;
	}
}

float LuaInstance::PopFloat(const char* var)
{
	if (!lua_isnumber(this->luaState, -1))
	{
		// Log error.
		std::string errorMessage = var;
		errorMessage += " is not a number.";
		this->LogError(errorMessage.c_str());
		lua_pop(this->luaState, 1);
		return false;
	}
	else
	{
		// Convert and return value.
		auto result = (float)lua_tonumber(this->luaState, -1);
		lua_pop(this->luaState, 1);
		return result;
	}
}

std::string LuaInstance::PopString(const char* var)
{
	if (!lua_isstring(this->luaState, -1))
	{
		// Log error.
		std::string errorMessage = var;
		errorMessage += " is not a string.";
		this->LogError(errorMessage.c_str());
		lua_pop(this->luaState, 1);
		return "";
	}
	else
	{
		// Convert and return value.
		auto result = lua_tostring(this->luaState, -1);
		lua_pop(this->luaState, 1);
		return result;
	}
}

void LuaInstance::LogError(const char* message)
{
	if (this->logger != nullptr)
	{
		// Print error message.
		this->logger->LogError(StringToWString(message));
	}
}