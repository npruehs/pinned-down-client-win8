
#include "pch.h"

#include "Actions\EndTurnAction.h"

#include "Data\TurnPhase.h"

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
}

void GameScreen::UnloadResources()
{
	this->game->resourceManager->UnloadResource("Assets/Window.png");
	this->game->resourceManager->UnloadResource("Assets/Button.png");
}

void GameScreen::LoadUI()
{
	// Player name label.
	this->playerNameLabel = this->uiFactory->CreateLabel(L"?");
	this->uiFactory->SetAnchor(this->playerNameLabel, VerticalAnchor(VerticalAnchorType::Top, 20.0f), HorizontalAnchor(HorizontalAnchorType::Right, -150.0f), 0);
	this->uiFactory->FinishUIWidget(this->playerNameLabel);

	// Distance label.
	this->distanceLabel = this->uiFactory->CreateLabel(L"Distance Covered: 0 / 0");
	this->uiFactory->SetAnchor(this->distanceLabel, VerticalAnchor(VerticalAnchorType::Top, 20.0f), HorizontalAnchor(HorizontalAnchorType::Left, 0.0f), this->playerNameLabel);
	this->uiFactory->FinishUIWidget(this->distanceLabel);

	// Turn Phase label.
	this->turnPhaseLabel = this->uiFactory->CreateLabel(L"Turn Phase:");
	this->uiFactory->SetAnchor(this->turnPhaseLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 20.0f), HorizontalAnchor(HorizontalAnchorType::Left, 0.0f), this->distanceLabel);
	this->uiFactory->FinishUIWidget(this->turnPhaseLabel);

	// Threat label.
	this->threatLabel = this->uiFactory->CreateLabel(L"Threat:");
	this->uiFactory->SetAnchor(this->threatLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 20.0f), HorizontalAnchor(HorizontalAnchorType::Left, 0.0f), this->turnPhaseLabel);
	this->uiFactory->FinishUIWidget(this->threatLabel);

	// End turn button.
	this->endTurnButton = this->uiFactory->CreateSprite("Assets/Button.png");
	this->uiFactory->SetAnchor(this->endTurnButton, VerticalAnchor(VerticalAnchorType::Top, 20.0f), HorizontalAnchor(HorizontalAnchorType::Left, 20.0f), 0);
	this->uiFactory->SetTappable(this->endTurnButton);
	this->uiFactory->FinishUIWidget(this->endTurnButton);

	this->endTurnLabel = this->uiFactory->CreateLabel(L"End Turn");
	this->uiFactory->SetAnchor(this->endTurnLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), this->endTurnButton);
	this->uiFactory->FinishUIWidget(this->endTurnLabel);
}

void GameScreen::UnloadUI()
{
	this->game->entityManager->RemoveEntity(this->endTurnLabel);
	this->game->entityManager->RemoveEntity(this->endTurnButton);
	this->game->entityManager->RemoveEntity(this->threatLabel);
	this->game->entityManager->RemoveEntity(this->turnPhaseLabel);
	this->game->entityManager->RemoveEntity(this->distanceLabel);
	this->game->entityManager->RemoveEntity(this->playerNameLabel);
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

	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->distanceLabel, TextComponent::TextComponentType);
	textComponent->text = L"Distance Covered: " + std::to_wstring(distanceCovered) + L" / " + std::to_wstring(distanceMaximum);
}

void GameScreen::OnDefeat(DefeatEvent& defeatEvent)
{
	// Show defeat window.
	this->ShowGameOver(L"DEFEAT!");
}

void GameScreen::OnEntityTapped(EntityTappedEvent& entityTappedEvent)
{
	if (entityTappedEvent.entity == this->endTurnButton)
	{
		auto endTurnAction = std::make_shared<EndTurnAction>();
		this->game->eventManager->QueueEvent(endTurnAction);
	}
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
	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->threatLabel, TextComponent::TextComponentType);
	textComponent->text = L"Threat: " + std::to_wstring(threatChangedEvent.newThreat);
}

void GameScreen::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	std::wstring turnPhaseName = StringToWString(TurnPhaseToString(turnPhaseChangedEvent.newTurnPhase));

	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->turnPhaseLabel, TextComponent::TextComponentType);
	textComponent->text = L"Turn Phase: " + turnPhaseName;
}

void GameScreen::OnVictory(VictoryEvent& victoryEvent)
{
	// Show victory window.
	this->ShowGameOver(L"VICTORY!");
}

void GameScreen::ShowGameOver(std::wstring title)
{
	// Remove End Turn button.
	this->game->entityManager->RemoveEntity(this->endTurnButton);
	this->game->entityManager->RemoveEntity(this->endTurnLabel);

	// Show victory window.
	this->gameOverWindow = this->uiFactory->CreateSprite("Assets/Window.png");
	this->uiFactory->SetAnchor(this->gameOverWindow, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->FinishUIWidget(this->gameOverWindow);

	this->gameOverLabel = this->uiFactory->CreateLabel(title);
	this->uiFactory->SetFontSize(this->gameOverLabel, 72.0f);
	this->uiFactory->SetAnchor(this->gameOverLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), this->gameOverWindow);
	this->uiFactory->FinishUIWidget(this->gameOverLabel);
}