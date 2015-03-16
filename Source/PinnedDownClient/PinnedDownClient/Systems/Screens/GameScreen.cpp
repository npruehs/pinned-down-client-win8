
#include "pch.h"

#include "Actions\PlayerReadyAction.h"

#include "Events\LocalizedTextChangedEvent.h"
#include "Events\MatchEndedEvent.h"

#include "Data\TurnPhase.h"

#include "Source\Features\Rendering\Components\ColorComponent.h"
#include "Source\Features\Globalization\Components\LocalizationComponent.h"
#include "Source\Features\Globalization\Components\TextComponent.h"

#include "Resources\PinnedDownResourceManager.h"

#include "Systems\Screens\GameScreen.h"

#include "Util\StringUtils.h"


using namespace PinnedDownNet::Events;
using namespace PinnedDownNet::Data;
using namespace PinnedDownClient::Features::Globalization;
using namespace PinnedDownClient::Features::Rendering;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Resources;
using namespace PinnedDownClient::Systems::Screens;
using namespace PinnedDownClient::Util;


GameScreen::GameScreen()
{
}

ScreenName GameScreen::GetScreenName()
{
	return ScreenName::Game;
}

void GameScreen::InitScreen(PinnedDownCore::Game* game, std::shared_ptr<ClientIdMapping> clientIdMapping, std::shared_ptr<EntityIdMapping> entityIdMapping)
{
	Screen::InitScreen(game, clientIdMapping, entityIdMapping);

	this->game->eventManager->AddListener(this, CoveredDistanceChangedEvent::CoveredDistanceChangedEventType);
	this->game->eventManager->AddListener(this, DefeatEvent::DefeatEventType);
	this->game->eventManager->AddListener(this, EntityTappedEvent::EntityTappedEventType);
	this->game->eventManager->AddListener(this, ErrorMessageEvent::ErrorMessageEventType);
	this->game->eventManager->AddListener(this, PlayerLeftEvent::PlayerLeftEventType);
	this->game->eventManager->AddListener(this, PlayerReadyStateResetEvent::PlayerReadyStateResetEventType);
	this->game->eventManager->AddListener(this, ThreatChangedEvent::ThreatChangedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
	this->game->eventManager->AddListener(this, VictoryEvent::VictoryEventType);
	this->game->eventManager->AddListener(this, WatchedPlayerChangedEvent::WatchedPlayerChangedEventType);
}

void GameScreen::DeInitScreen()
{
	this->game->eventManager->RemoveListener(this, CoveredDistanceChangedEvent::CoveredDistanceChangedEventType);
	this->game->eventManager->RemoveListener(this, DefeatEvent::DefeatEventType);
	this->game->eventManager->RemoveListener(this, EntityTappedEvent::EntityTappedEventType);
	this->game->eventManager->RemoveListener(this, ErrorMessageEvent::ErrorMessageEventType);
	this->game->eventManager->RemoveListener(this, PlayerLeftEvent::PlayerLeftEventType);
	this->game->eventManager->RemoveListener(this, PlayerReadyStateResetEvent::PlayerReadyStateResetEventType);
	this->game->eventManager->RemoveListener(this, ThreatChangedEvent::ThreatChangedEventType);
	this->game->eventManager->RemoveListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
	this->game->eventManager->RemoveListener(this, VictoryEvent::VictoryEventType);
	this->game->eventManager->RemoveListener(this, WatchedPlayerChangedEvent::WatchedPlayerChangedEventType);
}

void GameScreen::LoadResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext)
{
	auto resourceManager = static_cast<PinnedDownResourceManager*>(this->game->resourceManager.get());

	resourceManager->LoadBitmapFromFile(
		d2dContext.Get(),
		L"Assets/Window.png"
		);
	resourceManager->LoadBitmapFromFile(
		d2dContext.Get(),
		L"Assets/Button.png"
		);
	resourceManager->LoadBitmapFromFile(
		d2dContext.Get(),
		L"Assets/Hints.png"
		);
}

void GameScreen::UnloadResources()
{
	this->game->resourceManager->UnloadResource("Assets/Window.png");
	this->game->resourceManager->UnloadResource("Assets/Button.png");
	this->game->resourceManager->UnloadResource("Assets/Hints.png");
}

void GameScreen::LoadUI()
{
	// Player name label.
	this->playerNameLabel = this->uiFactory->CreateLabel();
	this->uiFactory->SetAnchor(this->playerNameLabel, VerticalAnchor(VerticalAnchorType::Top, 0.022f), HorizontalAnchor(HorizontalAnchorType::Left, 0.875f), 0);
	this->uiFactory->FinishUIWidget(this->playerNameLabel);

	this->playerNameYouLabel = this->uiFactory->CreateLabel("GameScreen_Info_PlayerName_You");
	this->uiFactory->SetAnchor(this->playerNameYouLabel, VerticalAnchor(VerticalAnchorType::Top, 0.0f), HorizontalAnchor(HorizontalAnchorType::Left, 0.075f), this->playerNameLabel);
	this->uiFactory->FinishUIWidget(this->playerNameYouLabel);

	// Distance label.
	this->distanceLabel = this->uiFactory->CreateLabel("GameScreen_Info_DistanceCovered");
	this->uiFactory->SetAnchor(this->distanceLabel, VerticalAnchor(VerticalAnchorType::Top, 0.022f), HorizontalAnchor(HorizontalAnchorType::Left, 0.0f), this->playerNameLabel);
	this->uiFactory->FinishUIWidget(this->distanceLabel);

	this->distanceValueLabel = this->uiFactory->CreateLabel();
	this->uiFactory->SetAnchor(this->distanceValueLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::Left, 0.075f), this->distanceLabel);
	this->uiFactory->FinishUIWidget(this->distanceValueLabel);

	// Turn Phase label.
	this->turnPhaseLabel = this->uiFactory->CreateLabel("GameScreen_Info_TurnPhase");
	this->uiFactory->SetAnchor(this->turnPhaseLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.022f), HorizontalAnchor(HorizontalAnchorType::Left, 0.0f), this->distanceLabel);
	this->uiFactory->FinishUIWidget(this->turnPhaseLabel);

	this->turnPhaseValueLabel = this->uiFactory->CreateLabel("");
	this->uiFactory->SetAnchor(this->turnPhaseValueLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::Left, 0.075f), this->turnPhaseLabel);
	this->uiFactory->FinishUIWidget(this->turnPhaseValueLabel);

	// Threat label.
	this->threatLabel = this->uiFactory->CreateLabel("GameScreen_Info_Threat");
	this->uiFactory->SetAnchor(this->threatLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.022f), HorizontalAnchor(HorizontalAnchorType::Left, 0.0f), this->turnPhaseLabel);
	this->uiFactory->FinishUIWidget(this->threatLabel);

	this->threatValueLabel = this->uiFactory->CreateLabel("");
	this->uiFactory->SetAnchor(this->threatValueLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::Left, 0.075f), this->threatLabel);
	this->uiFactory->FinishUIWidget(this->threatValueLabel);

	// End turn button.
	this->endTurnButton = this->uiFactory->CreateSprite("Assets/Button.png");
	this->uiFactory->SetAnchor(this->endTurnButton, VerticalAnchor(VerticalAnchorType::Top, 0.022f), HorizontalAnchor(HorizontalAnchorType::Left, 0.0125f), 0);
	this->uiFactory->SetTappable(this->endTurnButton, true);
	this->uiFactory->FinishUIWidget(this->endTurnButton);

	this->endTurnLabel = this->uiFactory->CreateLabel("GameScreen_Button_EndTurnPhase_NotReady");
	this->uiFactory->SetAnchor(this->endTurnLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), this->endTurnButton);
	this->uiFactory->FinishUIWidget(this->endTurnLabel);

	// Hints button.
	this->hintButton = this->uiFactory->CreateSprite("Assets/Button.png");
	this->uiFactory->SetAnchor(this->hintButton, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::Right, 0.1f), this->endTurnButton);
	this->uiFactory->SetTappable(this->hintButton, true);
	this->uiFactory->FinishUIWidget(this->hintButton);

	this->hintLabel = this->uiFactory->CreateLabel("GameScreen_Button_Hint");
	this->uiFactory->SetAnchor(this->hintLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), this->hintButton);
	this->uiFactory->FinishUIWidget(this->hintLabel);

	// Turn Phase Hint label.
	this->turnPhaseHintLabel = this->uiFactory->CreateLabel("");
	this->uiFactory->SetAnchor(this->turnPhaseHintLabel, VerticalAnchor(VerticalAnchorType::Top, 0.022f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->FinishUIWidget(this->turnPhaseHintLabel);

	// Error Message label.
	this->errorMessageLabel = this->uiFactory->CreateLabel("");
	this->uiFactory->SetAnchor(this->errorMessageLabel, VerticalAnchor(VerticalAnchorType::Top, 0.0556f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), this->turnPhaseHintLabel);
	this->uiFactory->SetColor(this->errorMessageLabel, D2D1::ColorF(D2D1::ColorF::Red));
	this->uiFactory->FinishUIWidget(this->errorMessageLabel);

	// Hint overlay.
	this->hintOverlay = this->uiFactory->CreateSprite("Assets/Hints.png");
	this->uiFactory->SetAnchor(this->hintOverlay, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f));
	this->uiFactory->SetDepth(this->hintOverlay, 10);
	this->uiFactory->SetVisible(this->hintOverlay, true);
	this->uiFactory->SetTappable(this->hintOverlay, true);
	this->uiFactory->FinishUIWidget(this->hintOverlay);
}

void GameScreen::UnloadUI()
{
	this->game->entityManager->RemoveEntity(this->distanceLabel);
	this->game->entityManager->RemoveEntity(this->distanceValueLabel);
	this->game->entityManager->RemoveEntity(this->errorMessageLabel);
	this->game->entityManager->RemoveEntity(this->endGameButton);
	this->game->entityManager->RemoveEntity(this->endGameLabel);
	this->game->entityManager->RemoveEntity(this->endTurnButton);
	this->game->entityManager->RemoveEntity(this->endTurnLabel);
	this->game->entityManager->RemoveEntity(this->gameOverLabel);
	this->game->entityManager->RemoveEntity(this->gameOverWindow);
	this->game->entityManager->RemoveEntity(this->hintButton);
	this->game->entityManager->RemoveEntity(this->hintLabel);
	this->game->entityManager->RemoveEntity(this->hintOverlay);
	this->game->entityManager->RemoveEntity(this->playerNameLabel);
	this->game->entityManager->RemoveEntity(this->playerNameYouLabel);
	this->game->entityManager->RemoveEntity(this->threatLabel);
	this->game->entityManager->RemoveEntity(this->threatValueLabel);
	this->game->entityManager->RemoveEntity(this->turnPhaseLabel);
	this->game->entityManager->RemoveEntity(this->turnPhaseHintLabel);
	this->game->entityManager->RemoveEntity(this->turnPhaseValueLabel);
}

void GameScreen::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == CoveredDistanceChangedEvent::CoveredDistanceChangedEventType)
	{
		auto coveredDistanceChangedEvent = static_cast<CoveredDistanceChangedEvent&>(newEvent);
		this->OnCoveredDistanceChanged(coveredDistanceChangedEvent);
	}
	else if (newEvent.GetEventType() == DefeatEvent::DefeatEventType)
	{
		auto defeatEvent = static_cast<DefeatEvent&>(newEvent);
		this->OnDefeat(defeatEvent);
	}
	else if (newEvent.GetEventType() == EntityTappedEvent::EntityTappedEventType)
	{
		auto entityTappedEvent = static_cast<EntityTappedEvent&>(newEvent);
		this->OnEntityTapped(entityTappedEvent);
	}
	else if (newEvent.GetEventType() == ErrorMessageEvent::ErrorMessageEventType)
	{
		auto errorMessageEvent = static_cast<ErrorMessageEvent&>(newEvent);
		this->OnErrorMessage(errorMessageEvent);
	}
	else if (newEvent.GetEventType() == PlayerLeftEvent::PlayerLeftEventType)
	{
		auto playerLeftEvent = static_cast<PlayerLeftEvent&>(newEvent);
		this->OnPlayerLeft(playerLeftEvent);
	}
	else if (newEvent.GetEventType() == PlayerReadyStateResetEvent::PlayerReadyStateResetEventType)
	{
		auto playerReadyStateResetEvent = static_cast<PlayerReadyStateResetEvent&>(newEvent);
		this->OnPlayerReadyStateReset(playerReadyStateResetEvent);
	}
	else if (newEvent.GetEventType() == ThreatChangedEvent::ThreatChangedEventType)
	{
		auto threatChangedEvent = static_cast<ThreatChangedEvent&>(newEvent);
		this->OnThreatChanged(threatChangedEvent);
	}
	else if (newEvent.GetEventType() == TurnPhaseChangedEvent::TurnPhaseChangedEventType)
	{
		auto turnPhaseChangedEvent = static_cast<TurnPhaseChangedEvent&>(newEvent);
		this->OnTurnPhaseChanged(turnPhaseChangedEvent);
	}
	else if (newEvent.GetEventType() == VictoryEvent::VictoryEventType)
	{
		auto victoryEvent = static_cast<VictoryEvent&>(newEvent);
		this->OnVictory(victoryEvent);
	}
	else if (newEvent.GetEventType() == WatchedPlayerChangedEvent::WatchedPlayerChangedEventType)
	{
		auto watchedPlayerChangedEvent = static_cast<WatchedPlayerChangedEvent&>(newEvent);
		this->OnWatchedPlayerChanged(watchedPlayerChangedEvent);
	}
}

void GameScreen::Update(float dt)
{
	if (this->errorTimeRemaining <= 0)
	{
		return;
	}

	// Tick error message timer.
	this->errorTimeRemaining -= dt;

	// Fade error message out.
	auto colorComponent = this->game->entityManager->GetComponent<ColorComponent>(this->errorMessageLabel, ColorComponent::ColorComponentType);
	colorComponent->color.a = this->errorTimeRemaining / this->errorTimeout;
}

void GameScreen::OnCoveredDistanceChanged(CoveredDistanceChangedEvent& coveredDistanceChangedEvent)
{
	int distanceCovered = coveredDistanceChangedEvent.distanceCovered;
	int distanceMaximum = coveredDistanceChangedEvent.distanceMaximum;

	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->distanceValueLabel, TextComponent::TextComponentType);
	textComponent->text = std::to_string(distanceCovered) + " / " + std::to_string(distanceMaximum);
}

void GameScreen::OnDefeat(DefeatEvent& defeatEvent)
{
	// Show defeat window.
	this->ShowGameOver("GameScreen_GameOver_Defeat");
}

void GameScreen::OnEntityTapped(EntityTappedEvent& entityTappedEvent)
{
	if (entityTappedEvent.entity == this->endTurnButton)
	{
		this->SetPlayerReady(!this->playerReady);

		// Player ready to end turn?
		auto endTurnAction = std::make_shared<PlayerReadyAction>(this->playerReady);
		this->game->eventManager->QueueEvent(endTurnAction);
	}
	else if (entityTappedEvent.entity == this->hintButton)
	{
		this->uiFactory->SetVisible(this->hintOverlay, true);
		this->uiFactory->SetTappable(this->hintOverlay, true);
	}
	else if (entityTappedEvent.entity == this->hintOverlay)
	{
		this->uiFactory->SetVisible(this->hintOverlay, false);
		this->uiFactory->SetTappable(this->hintOverlay, false);
	}
	else if (entityTappedEvent.entity == this->endGameButton)
	{
		auto matchEndedEvent = std::make_shared<MatchEndedEvent>();
		this->game->eventManager->QueueEvent(matchEndedEvent);
	}
}

void GameScreen::OnErrorMessage(ErrorMessageEvent& errorMessageEvent)
{
	// Set error message text.
	auto localizationComponent = this->game->entityManager->GetComponent<LocalizationComponent>(this->errorMessageLabel, LocalizationComponent::LocalizationComponentType);
	localizationComponent->localizationKey = errorMessageEvent.errorMessage;

	auto localizedTextChangedEvent = std::make_shared<LocalizedTextChangedEvent>(this->errorMessageLabel);
	this->game->eventManager->QueueEvent(localizedTextChangedEvent);

	// Start fading out.
	this->errorTimeRemaining = this->errorTimeout;
}

void GameScreen::OnPlayerLeft(PlayerLeftEvent& playerLeftEvent)
{
	// Show defeat window.
	this->ShowGameOver("GameScreen_GameOver_PlayerLeft");
}

void GameScreen::OnPlayerReadyStateReset(PlayerReadyStateResetEvent& playerReadyStateResetEvent)
{
	this->SetPlayerReady(false);
}

void GameScreen::OnThreatChanged(ThreatChangedEvent& threatChangedEvent)
{
	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->threatValueLabel, TextComponent::TextComponentType);
	textComponent->text = std::to_string(threatChangedEvent.newThreat);
}

void GameScreen::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	// Update phase.
	std::string turnPhaseNameKey = "GameScreen_TurnPhase_" + TurnPhaseToString(turnPhaseChangedEvent.newTurnPhase);

	auto localizationComponent = this->game->entityManager->GetComponent<LocalizationComponent>(this->turnPhaseValueLabel, LocalizationComponent::LocalizationComponentType);
	localizationComponent->localizationKey = turnPhaseNameKey;

	auto localizedTextChangedEvent = std::make_shared<LocalizedTextChangedEvent>(this->turnPhaseValueLabel);
	this->game->eventManager->QueueEvent(localizedTextChangedEvent);

	// Update hint.
	std::string turnPhaseHintKey = "GameScreen_Hint_" + TurnPhaseToString(turnPhaseChangedEvent.newTurnPhase);

	localizationComponent = this->game->entityManager->GetComponent<LocalizationComponent>(this->turnPhaseHintLabel, LocalizationComponent::LocalizationComponentType);
	localizationComponent->localizationKey = turnPhaseHintKey;

	localizedTextChangedEvent = std::make_shared<LocalizedTextChangedEvent>(this->turnPhaseHintLabel);
	this->game->eventManager->QueueEvent(localizedTextChangedEvent);
}

void GameScreen::OnVictory(VictoryEvent& victoryEvent)
{
	// Show victory window.
	this->ShowGameOver("GameScreen_GameOver_Victory");
}

void GameScreen::OnWatchedPlayerChanged(WatchedPlayerChangedEvent& watchedPlayerChangedEvent)
{
	auto playerNameTextComponent = this->game->entityManager->GetComponent<TextComponent>(this->playerNameLabel, TextComponent::TextComponentType);
	playerNameTextComponent->text = watchedPlayerChangedEvent.playerName;
	
	if (this->clientIdMapping->IsLocalPlayer(watchedPlayerChangedEvent.serverEntity))
	{
		// Show "You!".
		auto localizedTextChangedEvent = std::make_shared<LocalizedTextChangedEvent>(this->playerNameYouLabel);
		this->game->eventManager->QueueEvent(localizedTextChangedEvent);
	}
	else
	{
		// Hide "You!".
		auto playerNameYouTextComponent = this->game->entityManager->GetComponent<TextComponent>(this->playerNameYouLabel, TextComponent::TextComponentType);
		playerNameYouTextComponent->text = "";
	}
}

void GameScreen::SetPlayerReady(bool ready)
{
	if (this->gameOver)
	{
		return;
	}

	this->playerReady = ready;

	// Update button label.
	auto localizationComponent = this->game->entityManager->GetComponent<LocalizationComponent>(this->endTurnLabel, LocalizationComponent::LocalizationComponentType);
	localizationComponent->localizationKey = ready ? "GameScreen_Button_EndTurnPhase_Ready" : "GameScreen_Button_EndTurnPhase_NotReady";

	auto localizedTextChangedEvent = std::make_shared<LocalizedTextChangedEvent>(this->endTurnLabel);
	this->game->eventManager->QueueEvent(localizedTextChangedEvent);
}

void GameScreen::ShowGameOver(std::string title)
{
	this->gameOver = true;

	// Remove End Turn button.
	this->game->entityManager->RemoveEntity(this->endTurnButton);
	this->game->entityManager->RemoveEntity(this->endTurnLabel);

	this->game->entityManager->RemoveEntity(this->hintButton);
	this->game->entityManager->RemoveEntity(this->hintLabel);

	// Show victory window.
	this->gameOverWindow = this->uiFactory->CreateSprite("Assets/Window.png");
	this->uiFactory->SetAnchor(this->gameOverWindow, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->FinishUIWidget(this->gameOverWindow);

	this->gameOverLabel = this->uiFactory->CreateLabel(title);
	this->uiFactory->SetFontSize(this->gameOverLabel, 72.0f);
	this->uiFactory->SetAnchor(this->gameOverLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), this->gameOverWindow);
	this->uiFactory->FinishUIWidget(this->gameOverLabel);

	// Show end game button.
	this->endGameButton = this->uiFactory->CreateSprite("Assets/Button.png");
	this->uiFactory->SetAnchor(this->endGameButton, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.1f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), this->gameOverWindow);
	this->uiFactory->SetTappable(this->endGameButton, true);
	this->uiFactory->FinishUIWidget(this->endGameButton);

	this->endGameLabel = this->uiFactory->CreateLabel("GameScreen_GameOver_EndGame");
	this->uiFactory->SetAnchor(this->endGameLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), this->endGameButton);
	this->uiFactory->FinishUIWidget(this->endGameLabel);
}