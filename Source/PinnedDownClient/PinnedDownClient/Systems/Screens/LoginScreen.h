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

				void InitScreen(PinnedDownCore::Game* game, std::shared_ptr<ClientIdMapping> clientIdMapping, std::shared_ptr<EntityIdMapping> entityIdMapping);
				void DeInitScreen();

				void LoadResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext);
				void UnloadResources();

				void LoadUI();
				void UnloadUI();

				void Update(float dt);

				ScreenName GetScreenName();

			private:
				Entity splashScreen = INVALID_ENTITY_ID;
				Entity statusLabel = INVALID_ENTITY_ID;
				Entity reconnectButton = INVALID_ENTITY_ID;
				Entity reconnectLabel = INVALID_ENTITY_ID;

				bool connecting = false;
				bool showReconnectUI = false;

				float totalTime = 0.0f;

				void OnEvent(Event & event);

				void DoLogin();

				void OnEntityTapped(EntityTappedEvent& entityTappedEvent);
				void OnLoginError(LoginErrorEvent& loginErrorEvent);
			};
		}
	}
}