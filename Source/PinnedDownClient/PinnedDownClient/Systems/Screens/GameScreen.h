#pragma once

#include "Events\CoveredDistanceChangedEvent.h"
#include "Events\EntityTappedEvent.h"
#include "Events\TurnPhaseChangedEvent.h"

#include "Systems\Screens\Screen.h"

using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Events;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace Screens
		{
			class GameScreen : public Screen, public IEventListener
			{
			public:
				GameScreen();

				void InitScreen(PinnedDownCore::Game* game);
				void DeInitScreen();

				void LoadResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext);
				void UnloadResources();

				void LoadUI();
				void UnloadUI();

				ScreenName GetScreenName();

			private:
				int distanceLabel = INVALID_ENTITY_ID;
				int turnPhaseLabel = INVALID_ENTITY_ID;
				int endTurnButton = INVALID_ENTITY_ID;
				int endTurnLabel = INVALID_ENTITY_ID;

				void OnEvent(Event & event);

				void OnCoveredDistanceChanged(CoveredDistanceChangedEvent& coveredDistanceChangedEvent);
				void OnEntityTapped(EntityTappedEvent& entityTappedEvent);
				void OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent);
			};
		}
	}
}