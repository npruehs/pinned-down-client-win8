#pragma once

#include "Events\LoginErrorEvent.h"

#include "Systems\Screens\Screen.h"

using namespace PinnedDownClient::Core;
using namespace PinnedDownClient::Events;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace Screens
		{
			class LoginScreen : public Screen, public IEventListener
			{
			public:
				LoginScreen();

				void InitScreen(std::shared_ptr<GameInfrastructure> game);
				void DeInitScreen();

				void LoadUI();
				void UnloadUI();

				ScreenName GetScreenName();

			private:
				int statusLabel;

				void OnEvent(Event & event);

				void OnLoginSuccess();
				void OnLoginError(LoginErrorEvent& loginErrorEvent);
			};
		}
	}
}