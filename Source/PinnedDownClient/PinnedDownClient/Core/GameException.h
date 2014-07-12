#pragma once

#include <exception>

using namespace std;

namespace PinnedDownClient
{
	namespace Core
	{
		class GameException : public runtime_error
		{
		public:
			GameException(const char *msg)
				: runtime_error(msg)
			{
			}

			GameException(std::string msg)
				: runtime_error(msg.c_str())
			{
			}
		};
	}
}