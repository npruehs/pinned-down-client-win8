#pragma once

#include "Events\EntityTappedEvent.h"
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

				void LoadResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext);
				void UnloadResources();

				void LoadUI();
				void UnloadUI();

				void Update(float dt);

				ScreenName GetScreenName();

			private:
				int splashScreen;
				int statusLabel;
				int reconnectButton;
				int reconnectLabel;

				bool connecting;
				float totalTime;

				void OnEvent(Event & event);

				void DoLogin();

				void OnEntityTapped(EntityTappedEvent& entityTappedEvent);
				void OnLoginError(LoginErrorEvent& loginErrorEvent);
			};
		}
	}
}