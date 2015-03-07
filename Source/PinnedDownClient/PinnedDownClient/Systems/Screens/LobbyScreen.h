#pragma once

#include "Systems\Screens\Screen.h"

using namespace PinnedDownClient::Events;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace Screens
		{
			class LobbyScreen : public Screen, public IEventListener
			{
			public:
				LobbyScreen();

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

				float totalTime = 0.0f;

				void OnEvent(Event & event);
			};
		}
	}
}