#pragma once

#include "Events\CoveredDistanceChangedEvent.h"
#include "Events\DefeatEvent.h"
#include "Events\EntityTappedEvent.h"
#include "Events\ErrorMessageEvent.h"
#include "Events\PlayerAddedEvent.h"
#include "Events\PlayerReadyStateResetEvent.h"
#include "Events\ThreatChangedEvent.h"
#include "Events\TurnPhaseChangedEvent.h"
#include "Events\VictoryEvent.h"

#include "Systems\Screens\Screen.h"

using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Util;


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

				void InitScreen(PinnedDownCore::Game* game, std::shared_ptr<ClientIdMapping> clientIdMapping, std::shared_ptr<EntityIdMapping> entityIdMapping);
				void DeInitScreen();

				void LoadResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext);
				void UnloadResources();

				void LoadUI();
				void UnloadUI();

				void Update(float dt);

				ScreenName GetScreenName();

			private:
				Entity distanceLabel = INVALID_ENTITY_ID;
				Entity distanceValueLabel = INVALID_ENTITY_ID;
				Entity errorMessageLabel = INVALID_ENTITY_ID;
				Entity endGameButton = INVALID_ENTITY_ID;
				Entity endGameLabel = INVALID_ENTITY_ID;
				Entity endTurnButton = INVALID_ENTITY_ID;
				Entity endTurnLabel = INVALID_ENTITY_ID;
				Entity gameOverLabel = INVALID_ENTITY_ID;
				Entity gameOverWindow = INVALID_ENTITY_ID;
				Entity hintButton = INVALID_ENTITY_ID;
				Entity hintLabel = INVALID_ENTITY_ID;
				Entity hintOverlay = INVALID_ENTITY_ID;
				Entity playerNameLabel = INVALID_ENTITY_ID;
				Entity threatLabel = INVALID_ENTITY_ID;
				Entity threatValueLabel = INVALID_ENTITY_ID;
				Entity turnPhaseLabel = INVALID_ENTITY_ID;
				Entity turnPhaseHintLabel = INVALID_ENTITY_ID;
				Entity turnPhaseValueLabel = INVALID_ENTITY_ID;

				// Time before error messages are hidden, in seconds.
				const float errorTimeout = 3.0f;

				// Time before the current message is hidden, in seconds.
				float errorTimeRemaining = 0.0f;

				void OnEvent(Event & event);

				void OnCoveredDistanceChanged(CoveredDistanceChangedEvent& coveredDistanceChangedEvent);
				void OnDefeat(DefeatEvent& defeatEvent);
				void OnEntityTapped(EntityTappedEvent& entityTappedEvent);
				void OnErrorMessage(ErrorMessageEvent& errorMessageEvent);
				void OnPlayerAdded(PlayerAddedEvent& playerAddedEvent);
				void OnPlayerReadyStateReset(PlayerReadyStateResetEvent& playerReadyStateResetEvent);
				void OnThreatChanged(ThreatChangedEvent& threatChangedEvent);
				void OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent);
				void OnVictory(VictoryEvent& victoryEvent);
				
				void ShowPlayerReady(bool ready);
				void ShowGameOver(std::string title);
			};
		}
	}
}