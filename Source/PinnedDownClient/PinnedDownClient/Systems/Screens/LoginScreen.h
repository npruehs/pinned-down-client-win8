#pragma once

#include "Systems\Screens\Screen.h"

using namespace PinnedDownClient::Core;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace Screens
		{
			class LoginScreen : public Screen
			{
			public:
				LoginScreen();

				void LoadUI();
				void UnloadUI();

				ScreenName GetScreenName();

			private:
				int statusLabel;
			};
		}
	}
}