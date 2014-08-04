#include "pch.h"

#include "Actions\ConnectToServerAction.h"

#include "Components\TextComponent.h"

#include "Resources\PinnedDownResourceManager.h"

#include "Systems\Screens\LoginScreen.h"


using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Resources;
using namespace PinnedDownClient::Systems::Screens;


LoginScreen::LoginScreen()
{
}

ScreenName LoginScreen::GetScreenName()
{
	return ScreenName::Login;
}


void LoginScreen::InitScreen(PinnedDownCore::Game* game)
{
	Screen::InitScreen(game);

	this->game->eventManager->AddListener(this, EntityTappedEvent::EntityTappedEventType);
	this->game->eventManager->AddListener(this, LoginErrorEvent::LoginErrorEventType);

	// Start connecting to server.
	this->DoLogin();
}

void LoginScreen::DeInitScreen()
{
	this->game->eventManager->RemoveListener(this, LoginErrorEvent::LoginErrorEventType);
}

void LoginScreen::Update(float dt)
{
	this->totalTime += dt;

	if (!this->connecting)
	{
		return;
	}

	std::wstring text = L"Connecting ";

	if (this->totalTime - floorf(this->totalTime) < 0.33)
	{
		text += L".";
	}
	else if (this->totalTime - floorf(this->totalTime) < 0.66)
	{
		text += L"..";
	}
	else
	{
		text += L"...";
	}

	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->statusLabel, TextComponent::TextComponentType);
	textComponent->text = text;
}

void LoginScreen::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == EntityTappedEvent::EntityTappedEventType)
	{
		auto entityTappedEvent = static_cast<EntityTappedEvent&>(newEvent);
		this->OnEntityTapped(entityTappedEvent);
	}
	else if (newEvent.GetEventType() == LoginErrorEvent::LoginErrorEventType)
	{
		auto loginErrorEvent = static_cast<LoginErrorEvent&>(newEvent);
		this->OnLoginError(loginErrorEvent);
	}
}

void LoginScreen::LoadResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext)
{
	auto resourceManager = static_cast<PinnedDownResourceManager*>(this->game->resourceManager.get());

	resourceManager->LoadBitmapFromFile(
		d2dContext.Get(),
		L"Assets/SplashScreen.png"
		);
	resourceManager->LoadBitmapFromFile(
		d2dContext.Get(),
		L"Assets/Button.png"
		);
}

void LoginScreen::UnloadResources()
{
	this->game->resourceManager->UnloadResource(L"Assets/SplashScreen.png");
	this->game->resourceManager->UnloadResource(L"Assets/Button.png");
}

void LoginScreen::LoadUI()
{
	// Status label.
	this->statusLabel = this->uiFactory->CreateLabel(L"Connecting...");
	this->uiFactory->SetAnchor(this->statusLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 200.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->SetFontSize(this->statusLabel, 18.0f);
	this->uiFactory->FinishUIWidget(this->statusLabel);

	// Logo.
	this->splashScreen = this->uiFactory->CreateSprite(L"Assets/SplashScreen.png");
	this->uiFactory->SetAnchor(this->splashScreen, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->FinishUIWidget(this->splashScreen);
}

void LoginScreen::UnloadUI()
{
	this->game->entityManager->RemoveEntity(this->statusLabel);
	this->game->entityManager->RemoveEntity(this->splashScreen);

	if (this->showReconnectUI)
	{
		this->game->entityManager->RemoveEntity(this->reconnectButton);
		this->game->entityManager->RemoveEntity(this->reconnectLabel);
	}
}

void LoginScreen::DoLogin()
{
	// Start connecting.
	auto connectToServerAction = std::make_shared<ConnectToServerAction>();
	this->game->eventManager->QueueEvent(connectToServerAction);

	this->connecting = true;

	// Hide reconnect button.
	if (this->showReconnectUI)
	{
		this->game->entityManager->RemoveEntity(this->reconnectButton);
		this->game->entityManager->RemoveEntity(this->reconnectLabel);

		this->showReconnectUI = false;
	}
}

void LoginScreen::OnEntityTapped(EntityTappedEvent& entityTappedEvent)
{
	if (entityTappedEvent.entityId == this->reconnectButton)
	{
		this->DoLogin();
	}
}

void LoginScreen::OnLoginError(LoginErrorEvent& loginErrorEvent)
{
	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->statusLabel, TextComponent::TextComponentType);
	textComponent->text = loginErrorEvent.errorMessage;

	this->connecting = false;

	// Add Reconnect button.
	this->reconnectButton = this->uiFactory->CreateSprite(L"Assets/Button.png");
	this->uiFactory->SetAnchor(this->reconnectButton, VerticalAnchor(VerticalAnchorType::VerticalCenter, 300.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->SetTappable(this->reconnectButton);
	this->uiFactory->FinishUIWidget(this->reconnectButton);

	this->reconnectLabel = this->uiFactory->CreateLabel(L"Reconnect");
	this->uiFactory->SetAnchor(this->reconnectLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), this->reconnectButton);
	this->uiFactory->SetFontSize(this->reconnectLabel, 18.0f);
	this->uiFactory->FinishUIWidget(this->reconnectLabel);

	this->showReconnectUI = true;
}