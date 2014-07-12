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
			GameException(const char *msg);
			GameException(std::wstring msg);
		};
	}
}