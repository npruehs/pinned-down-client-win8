#pragma once

#include "Events\CoveredDistanceChangedEvent.h"
#include "Events\DefeatEvent.h"
#include "Events\EntityTappedEvent.h"
#include "Events\ErrorMessageEvent.h"
#include "Events\PlayerAddedEvent.h"
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

				ScreenName GetScreenName();

			private:
				Entity distanceLabel = INVALID_ENTITY_ID;
				Entity turnPhaseLabel = INVALID_ENTITY_ID;
				Entity turnPhaseHintLabel = INVALID_ENTITY_ID;
				Entity threatLabel = INVALID_ENTITY_ID;
				Entity endTurnButton = INVALID_ENTITY_ID;
				Entity endTurnLabel = INVALID_ENTITY_ID;
				Entity errorMessageLabel = INVALID_ENTITY_ID;
				Entity gameOverWindow = INVALID_ENTITY_ID;
				Entity gameOverLabel = INVALID_ENTITY_ID;
				Entity playerNameLabel = INVALID_ENTITY_ID;
				
				void OnEvent(Event & event);

				void OnCoveredDistanceChanged(CoveredDistanceChangedEvent& coveredDistanceChangedEvent);
				void OnDefeat(DefeatEvent& defeatEvent);
				void OnEntityTapped(EntityTappedEvent& entityTappedEvent);
				void OnErrorMessage(ErrorMessageEvent& errorMessageEvent);
				void OnPlayerAdded(PlayerAddedEvent& playerAddedEvent);
				void OnThreatChanged(ThreatChangedEvent& threatChangedEvent);
				void OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent);
				void OnVictory(VictoryEvent& victoryEvent);
				
				void ShowGameOver(std::wstring title);
			};
		}
	}
}