
#include "pch.h"

#include "Actions\EndTurnAction.h"

#include "Components\TextComponent.h"

#include "Resources\PinnedDownResourceManager.h"

#include "Systems\Screens\GameScreen.h"


using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Components;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Resources;
using namespace PinnedDownClient::Systems::Screens;


GameScreen::GameScreen()
{
}

ScreenName GameScreen::GetScreenName()
{
	return ScreenName::Game;
}

void GameScreen::InitScreen(PinnedDownCore::Game* game)
{
	Screen::InitScreen(game);

	this->game->eventManager->AddListener(this, CoveredDistanceChangedEvent::CoveredDistanceChangedEventType);
	this->game->eventManager->AddListener(this, EntityTappedEvent::EntityTappedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void GameScreen::DeInitScreen()
{
	this->game->eventManager->RemoveListener(this, CoveredDistanceChangedEvent::CoveredDistanceChangedEventType);
	this->game->eventManager->RemoveListener(this, EntityTappedEvent::EntityTappedEventType);
	this->game->eventManager->RemoveListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
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
	// Distance label.
	this->distanceLabel = this->uiFactory->CreateLabel(L"Distance Covered: 0 / 0");
	this->uiFactory->SetAnchor(this->distanceLabel, VerticalAnchor(VerticalAnchorType::Top, 20.0f), HorizontalAnchor(HorizontalAnchorType::Right, -20.0f), 0);
	this->uiFactory->FinishUIWidget(this->distanceLabel);

	// Turn Phase label.
	this->turnPhaseLabel = this->uiFactory->CreateLabel(L"Turn Phase:");
	this->uiFactory->SetAnchor(this->turnPhaseLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 20.0f), HorizontalAnchor(HorizontalAnchorType::Left, 0.0f), this->distanceLabel);
	this->uiFactory->FinishUIWidget(this->turnPhaseLabel);

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
	this->game->entityManager->RemoveEntity(this->distanceLabel);
}

void GameScreen::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == CoveredDistanceChangedEvent::CoveredDistanceChangedEventType)
	{
		auto coveredDistanceChangedEvent = static_cast<CoveredDistanceChangedEvent&>(newEvent);
		this->OnCoveredDistanceChanged(coveredDistanceChangedEvent);
	}
	else if (newEvent.GetEventType() == EntityTappedEvent::EntityTappedEventType)
	{
		auto entityTappedEvent = static_cast<EntityTappedEvent&>(newEvent);
		this->OnEntityTapped(entityTappedEvent);
	}
	else if (newEvent.GetEventType() == TurnPhaseChangedEvent::TurnPhaseChangedEventType)
	{
		auto turnPhaseChangedEvent = static_cast<TurnPhaseChangedEvent&>(newEvent);
		this->OnTurnPhaseChanged(turnPhaseChangedEvent);
	}
}

void GameScreen::OnCoveredDistanceChanged(CoveredDistanceChangedEvent& coveredDistanceChangedEvent)
{
	int distanceCovered = coveredDistanceChangedEvent.distanceCovered;
	int distanceMaximum = coveredDistanceChangedEvent.distanceMaximum;

	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->distanceLabel, TextComponent::TextComponentType);
	textComponent->text = L"Distance Covered: " + std::to_wstring(distanceCovered) + L" / " + std::to_wstring(distanceMaximum);
}

void GameScreen::OnEntityTapped(EntityTappedEvent& entityTappedEvent)
{
	if (entityTappedEvent.entityId == this->endTurnButton)
	{
		auto endTurnAction = std::make_shared<EndTurnAction>();
		this->game->eventManager->QueueEvent(endTurnAction);
	}
}

void GameScreen::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	std::wstring turnPhaseName;
	
	switch (turnPhaseChangedEvent.newTurnPhase)
	{
	case TurnPhase::Assignment:
		turnPhaseName = L"Assignment";
		break;
	case TurnPhase::Attack:
		turnPhaseName = L"Attack";
		break;
	case TurnPhase::Fight:
		turnPhaseName = L"Fight";
		break;
	case TurnPhase::Jump:
		turnPhaseName = L"Jump";
		break;
	case TurnPhase::Main:
		turnPhaseName = L"Main";
		break;
	case TurnPhase::WrapUp:
		turnPhaseName = L"Wrap Up";
		break;
	default:
		turnPhaseName = L"Invalid";
		break;
	}

	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->turnPhaseLabel, TextComponent::TextComponentType);
	textComponent->text = L"Turn Phase: " + turnPhaseName;
}