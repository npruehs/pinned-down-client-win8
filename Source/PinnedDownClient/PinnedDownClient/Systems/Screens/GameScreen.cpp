
#include "pch.h"

#include "Actions\EndTurnAction.h"

#include "Events\LocalizedTextChangedEvent.h"

#include "Data\TurnPhase.h"

#include "Components\LocalizationComponent.h"
#include "Components\TextComponent.h"

#include "Resources\PinnedDownResourceManager.h"

#include "Systems\Screens\GameScreen.h"

#include "Util\StringUtils.h"


using namespace PinnedDownNet::Events;
using namespace PinnedDownNet::Data;
using namespace PinnedDownClient::Components;
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
	this->game->eventManager->AddListener(this, PlayerAddedEvent::PlayerAddedEventType);
	this->game->eventManager->AddListener(this, ThreatChangedEvent::ThreatChangedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
	this->game->eventManager->AddListener(this, VictoryEvent::VictoryEventType);	
}

void GameScreen::DeInitScreen()
{
	this->game->eventManager->RemoveListener(this, CoveredDistanceChangedEvent::CoveredDistanceChangedEventType);
	this->game->eventManager->RemoveListener(this, DefeatEvent::DefeatEventType);
	this->game->eventManager->RemoveListener(this, EntityTappedEvent::EntityTappedEventType);
	this->game->eventManager->RemoveListener(this, ErrorMessageEvent::ErrorMessageEventType);
	this->game->eventManager->RemoveListener(this, PlayerAddedEvent::PlayerAddedEventType);
	this->game->eventManager->RemoveListener(this, ThreatChangedEvent::ThreatChangedEventType);
	this->game->eventManager->RemoveListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
	this->game->eventManager->RemoveListener(this, VictoryEvent::VictoryEventType);
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
	this->playerNameLabel = this->uiFactory->CreateLabel(L"?");
	this->uiFactory->SetAnchor(this->playerNameLabel, VerticalAnchor(VerticalAnchorType::Top, 20.0f), HorizontalAnchor(HorizontalAnchorType::Right, -200.0f), 0);
	this->uiFactory->FinishUIWidget(this->playerNameLabel);

	// Distance label.
	this->distanceLabel = this->uiFactory->CreateLabel(L"GameScreen_Info_DistanceCovered");
	this->uiFactory->SetAnchor(this->distanceLabel, VerticalAnchor(VerticalAnchorType::Top, 20.0f), HorizontalAnchor(HorizontalAnchorType::Left, 0.0f), this->playerNameLabel);
	this->uiFactory->FinishUIWidget(this->distanceLabel);

	this->distanceValueLabel = this->uiFactory->CreateLabel(L"0 / 0");
	this->uiFactory->SetAnchor(this->distanceValueLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::Left, 120.0f), this->distanceLabel);
	this->uiFactory->FinishUIWidget(this->distanceValueLabel);

	// Turn Phase label.
	this->turnPhaseLabel = this->uiFactory->CreateLabel(L"GameScreen_Info_TurnPhase");
	this->uiFactory->SetAnchor(this->turnPhaseLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 20.0f), HorizontalAnchor(HorizontalAnchorType::Left, 0.0f), this->distanceLabel);
	this->uiFactory->FinishUIWidget(this->turnPhaseLabel);

	this->turnPhaseValueLabel = this->uiFactory->CreateLabel(L"");
	this->uiFactory->SetAnchor(this->turnPhaseValueLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::Left, 120.0f), this->turnPhaseLabel);
	this->uiFactory->FinishUIWidget(this->turnPhaseValueLabel);

	// Threat label.
	this->threatLabel = this->uiFactory->CreateLabel(L"GameScreen_Info_Threat");
	this->uiFactory->SetAnchor(this->threatLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 20.0f), HorizontalAnchor(HorizontalAnchorType::Left, 0.0f), this->turnPhaseLabel);
	this->uiFactory->FinishUIWidget(this->threatLabel);

	this->threatValueLabel = this->uiFactory->CreateLabel(L"");
	this->uiFactory->SetAnchor(this->threatValueLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::Left, 120.0f), this->threatLabel);
	this->uiFactory->FinishUIWidget(this->threatValueLabel);

	// End turn button.
	this->endTurnButton = this->uiFactory->CreateSprite("Assets/Button.png");
	this->uiFactory->SetAnchor(this->endTurnButton, VerticalAnchor(VerticalAnchorType::Top, 20.0f), HorizontalAnchor(HorizontalAnchorType::Left, 20.0f), 0);
	this->uiFactory->SetTappable(this->endTurnButton, true);
	this->uiFactory->FinishUIWidget(this->endTurnButton);

	this->endTurnLabel = this->uiFactory->CreateLabel(L"GameScreen_Button_EndTurnPhase");
	this->uiFactory->SetAnchor(this->endTurnLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), this->endTurnButton);
	this->uiFactory->FinishUIWidget(this->endTurnLabel);

	// Hints button.
	this->hintButton = this->uiFactory->CreateSprite("Assets/Button.png");
	this->uiFactory->SetAnchor(this->hintButton, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::Right, 200.0f), this->endTurnButton);
	this->uiFactory->SetTappable(this->hintButton, true);
	this->uiFactory->FinishUIWidget(this->hintButton);

	this->hintLabel = this->uiFactory->CreateLabel(L"GameScreen_Button_Hint");
	this->uiFactory->SetAnchor(this->hintLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), this->hintButton);
	this->uiFactory->FinishUIWidget(this->hintLabel);

	// Turn Phase Hint label.
	this->turnPhaseHintLabel = this->uiFactory->CreateLabel(L"");
	this->uiFactory->SetAnchor(this->turnPhaseHintLabel, VerticalAnchor(VerticalAnchorType::Top, 20.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->FinishUIWidget(this->turnPhaseHintLabel);

	// Error Message label.
	this->errorMessageLabel = this->uiFactory->CreateLabel(L"");
	this->uiFactory->SetAnchor(this->errorMessageLabel, VerticalAnchor(VerticalAnchorType::Top, 20.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), this->turnPhaseHintLabel);
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
	this->game->entityManager->RemoveEntity(this->endTurnLabel);
	this->game->entityManager->RemoveEntity(this->endTurnButton);
	this->game->entityManager->RemoveEntity(this->hintButton);
	this->game->entityManager->RemoveEntity(this->hintLabel);
	this->game->entityManager->RemoveEntity(this->threatLabel);
	this->game->entityManager->RemoveEntity(this->turnPhaseLabel);
	this->game->entityManager->RemoveEntity(this->distanceLabel);
	this->game->entityManager->RemoveEntity(this->playerNameLabel);
	this->game->entityManager->RemoveEntity(this->turnPhaseHintLabel);
	this->game->entityManager->RemoveEntity(this->errorMessageLabel);
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
	else if (newEvent.GetEventType() == PlayerAddedEvent::PlayerAddedEventType)
	{
		auto playerAddedEvent = static_cast<PlayerAddedEvent&>(newEvent);
		this->OnPlayerAdded(playerAddedEvent);
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
}

void GameScreen::OnCoveredDistanceChanged(CoveredDistanceChangedEvent& coveredDistanceChangedEvent)
{
	int distanceCovered = coveredDistanceChangedEvent.distanceCovered;
	int distanceMaximum = coveredDistanceChangedEvent.distanceMaximum;

	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->distanceValueLabel, TextComponent::TextComponentType);
	textComponent->text = std::to_wstring(distanceCovered) + L" / " + std::to_wstring(distanceMaximum);
}

void GameScreen::OnDefeat(DefeatEvent& defeatEvent)
{
	// Show defeat window.
	this->ShowGameOver(L"GameScreen_GameOver_Defeat");
}

void GameScreen::OnEntityTapped(EntityTappedEvent& entityTappedEvent)
{
	if (entityTappedEvent.entity == this->endTurnButton)
	{
		auto endTurnAction = std::make_shared<EndTurnAction>();
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
}

void GameScreen::OnErrorMessage(ErrorMessageEvent& errorMessageEvent)
{
	auto localizationComponent = this->game->entityManager->GetComponent<LocalizationComponent>(this->errorMessageLabel, LocalizationComponent::LocalizationComponentType);
	localizationComponent->localizationKey = StringToWString(errorMessageEvent.errorMessage);

	auto localizedTextChangedEvent = std::make_shared<LocalizedTextChangedEvent>(this->errorMessageLabel);
	this->game->eventManager->QueueEvent(localizedTextChangedEvent);
}

void GameScreen::OnPlayerAdded(PlayerAddedEvent& playerAddedEvent)
{
	if (this->clientIdMapping->IsLocalPlayer(playerAddedEvent.serverEntity))
	{
		auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->playerNameLabel, TextComponent::TextComponentType);
		textComponent->text = StringToWString(playerAddedEvent.playerName);
	}
}

void GameScreen::OnThreatChanged(ThreatChangedEvent& threatChangedEvent)
{
	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->threatValueLabel, TextComponent::TextComponentType);
	textComponent->text = std::to_wstring(threatChangedEvent.newThreat);
}

void GameScreen::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	// Update phase.
	std::wstring turnPhaseNameKey = L"GameScreen_TurnPhase_" + StringToWString(TurnPhaseToString(turnPhaseChangedEvent.newTurnPhase));

	auto localizationComponent = this->game->entityManager->GetComponent<LocalizationComponent>(this->turnPhaseValueLabel, LocalizationComponent::LocalizationComponentType);
	localizationComponent->localizationKey = turnPhaseNameKey;

	auto localizedTextChangedEvent = std::make_shared<LocalizedTextChangedEvent>(this->turnPhaseValueLabel);
	this->game->eventManager->QueueEvent(localizedTextChangedEvent);

	// Update hint.
	std::wstring turnPhaseHintKey = L"GameScreen_Hint_" + StringToWString(TurnPhaseToString(turnPhaseChangedEvent.newTurnPhase));

	localizationComponent = this->game->entityManager->GetComponent<LocalizationComponent>(this->turnPhaseHintLabel, LocalizationComponent::LocalizationComponentType);
	localizationComponent->localizationKey = turnPhaseHintKey;

	localizedTextChangedEvent = std::make_shared<LocalizedTextChangedEvent>(this->turnPhaseHintLabel);
	this->game->eventManager->QueueEvent(localizedTextChangedEvent);
}

void GameScreen::OnVictory(VictoryEvent& victoryEvent)
{
	// Show victory window.
	this->ShowGameOver(L"GameScreen_GameOver_Victory");
}

void GameScreen::ShowGameOver(std::wstring title)
{
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
}