#pragma once

#include "Events\LoginErrorEvent.h"

#include "Systems\Screens\Screen.h"

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

				void InitScreen(PinnedDownCore::Game* game);
				void DeInitScreen();

				void LoadUI();
				void UnloadUI();

				void Update(float dt);

				ScreenName GetScreenName();

			private:
				int statusLabel;
				bool connecting;
				float totalTime;

				void OnEvent(Event & event);

				void OnLoginError(LoginErrorEvent& loginErrorEvent);
			};
		}
	}
}