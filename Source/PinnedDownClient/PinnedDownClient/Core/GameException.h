#pragma once

#include <exception>

using namespace std;

namespace PinnedDownClient
{
	namespace Core
	{
		class GameException : public exception
		{
		public:
			GameException(const char *msg)
				: exception(msg)
			{
			}

			GameException(std::string msg)
				: exception(msg.c_str())
			{
			}
		};
	}
}