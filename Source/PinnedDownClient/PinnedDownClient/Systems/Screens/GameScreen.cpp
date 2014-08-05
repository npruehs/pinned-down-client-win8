
#include "pch.h"

#include "Components\TextComponent.h"

#include "Resources\PinnedDownResourceManager.h"

#include "Systems\Screens\GameScreen.h"


using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Components;
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
}

void GameScreen::DeInitScreen()
{
	this->game->eventManager->RemoveListener(this, CoveredDistanceChangedEvent::CoveredDistanceChangedEventType);
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
}

void GameScreen::OnCoveredDistanceChanged(CoveredDistanceChangedEvent& coveredDistanceChangedEvent)
{
	int distanceCovered = coveredDistanceChangedEvent.distanceCovered;
	int distanceMaximum = coveredDistanceChangedEvent.distanceMaximum;

	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->distanceLabel, TextComponent::TextComponentType);
	textComponent->text = L"Distance Covered: " + std::to_wstring(distanceCovered) + L" / " + std::to_wstring(distanceMaximum);
}