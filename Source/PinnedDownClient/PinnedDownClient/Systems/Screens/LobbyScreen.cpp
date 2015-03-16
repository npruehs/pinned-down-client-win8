#include "pch.h"

#include "Actions\JoinGameAction.h"

#include "Events\LocalizedTextChangedEvent.h"

#include "Source\Features\Globalization\Components\LocalizationComponent.h"
#include "Source\Features\Globalization\Components\TextComponent.h"

#include "Resources\PinnedDownResourceManager.h"

#include "Systems\Screens\LobbyScreen.h"


using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Features::Globalization;
using namespace PinnedDownClient::Resources;
using namespace PinnedDownClient::Systems::Screens;
using namespace PinnedDownNet::Events;


LobbyScreen::LobbyScreen()
{
}

ScreenName LobbyScreen::GetScreenName()
{
	return ScreenName::Lobby;
}


void LobbyScreen::InitScreen(PinnedDownCore::Game* game, std::shared_ptr<ClientIdMapping> clientIdMapping, std::shared_ptr<EntityIdMapping> entityIdMapping)
{
	Screen::InitScreen(game, clientIdMapping, entityIdMapping);
}

void LobbyScreen::DeInitScreen()
{
}

void LobbyScreen::Update(float dt)
{
	this->totalTime += dt;

	auto localizationComponent = this->game->entityManager->GetComponent<LocalizationComponent>(this->statusLabel, LocalizationComponent::LocalizationComponentType);
	std::string text = localizationComponent->localizationValue;

	if (this->totalTime - floorf(this->totalTime) < 0.33)
	{
		text += ".";
	}
	else if (this->totalTime - floorf(this->totalTime) < 0.66)
	{
		text += "..";
	}
	else
	{
		text += "...";
	}

	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->statusLabel, TextComponent::TextComponentType);
	textComponent->text = text;
}

void LobbyScreen::LoadResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext)
{
	auto resourceManager = static_cast<PinnedDownResourceManager*>(this->game->resourceManager.get());

	resourceManager->LoadBitmapFromFile(
		d2dContext.Get(),
		L"Assets/Logos/Splash.png"
		);
}

void LobbyScreen::UnloadResources()
{
	this->game->resourceManager->UnloadResource("Assets/Logos/Splash.png");
}

void LobbyScreen::LoadUI()
{
	// Status label.
	this->statusLabel = this->uiFactory->CreateLabel("LobbyScreen_WaitingForPlayers");
	this->uiFactory->SetAnchor(this->statusLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.222f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->SetFontSize(this->statusLabel, 22.0f);
	this->uiFactory->FinishUIWidget(this->statusLabel);

	// Logo.
	this->splashScreen = this->uiFactory->CreateSprite("Assets/Logos/Splash.png");
	this->uiFactory->SetAnchor(this->splashScreen, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->FinishUIWidget(this->splashScreen);

	// Join game.
	auto joinGameAction = std::make_shared<JoinGameAction>();
	this->game->eventManager->QueueEvent(joinGameAction);
}

void LobbyScreen::UnloadUI()
{
	this->game->entityManager->RemoveEntity(this->statusLabel);
	this->game->entityManager->RemoveEntity(this->splashScreen);
}

void LobbyScreen::OnEvent(Event & newEvent)
{
}
