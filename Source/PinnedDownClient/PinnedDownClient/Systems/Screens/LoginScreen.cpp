#include "pch.h"

#include "Actions\ConnectToServerAction.h"

#include "Events\LocalizedTextChangedEvent.h"

#include "Components\LocalizationComponent.h"
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


void LoginScreen::InitScreen(PinnedDownCore::Game* game, std::shared_ptr<ClientIdMapping> clientIdMapping, std::shared_ptr<EntityIdMapping> entityIdMapping)
{
	Screen::InitScreen(game, clientIdMapping, entityIdMapping);

	this->game->eventManager->AddListener(this, EntityTappedEvent::EntityTappedEventType);
	this->game->eventManager->AddListener(this, LoginErrorEvent::LoginErrorEventType);
}

void LoginScreen::DeInitScreen()
{
	this->game->eventManager->RemoveListener(this, EntityTappedEvent::EntityTappedEventType);
	this->game->eventManager->RemoveListener(this, LoginErrorEvent::LoginErrorEventType);
}

void LoginScreen::Update(float dt)
{
	this->totalTime += dt;

	if (!this->connecting)
	{
		return;
	}

	auto localizationComponent = this->game->entityManager->GetComponent<LocalizationComponent>(this->statusLabel, LocalizationComponent::LocalizationComponentType);
	std::wstring text = localizationComponent->localizationValue;

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

void LoginScreen::LoadResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext)
{
	auto resourceManager = static_cast<PinnedDownResourceManager*>(this->game->resourceManager.get());

	resourceManager->LoadBitmapFromFile(
		d2dContext.Get(),
		L"Assets/Logos/Splash.png"
		);
	resourceManager->LoadBitmapFromFile(
		d2dContext.Get(),
		L"Assets/Button.png"
		);
}

void LoginScreen::UnloadResources()
{
	this->game->resourceManager->UnloadResource("Assets/Logos/Splash.png");
	this->game->resourceManager->UnloadResource("Assets/Button.png");
}

void LoginScreen::LoadUI()
{
	// Status label.
	this->statusLabel = this->uiFactory->CreateLabel(L"LoginScreen_Connecting");
	this->uiFactory->SetAnchor(this->statusLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.222f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->SetFontSize(this->statusLabel, 22.0f);
	this->uiFactory->FinishUIWidget(this->statusLabel);

	// Logo.
	this->splashScreen = this->uiFactory->CreateSprite("Assets/Logos/Splash.png");
	this->uiFactory->SetAnchor(this->splashScreen, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->FinishUIWidget(this->splashScreen);

	// Start connecting to server.
	this->DoLogin();
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

void LoginScreen::DoLogin()
{
	// Start connecting.
	auto connectToServerAction = std::make_shared<ConnectToServerAction>();
	this->game->eventManager->QueueEvent(connectToServerAction);

	this->connecting = true;

	// Set status label text.
	auto localizationComponent = this->game->entityManager->GetComponent<LocalizationComponent>(this->statusLabel, LocalizationComponent::LocalizationComponentType);
	localizationComponent->localizationKey = L"LoginScreen_Connecting";

	auto localizedTextChangedEvent = std::make_shared<LocalizedTextChangedEvent>(this->statusLabel);
	this->game->eventManager->QueueEvent(localizedTextChangedEvent);

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
	if (entityTappedEvent.entity == this->reconnectButton)
	{
		this->DoLogin();
	}
}

void LoginScreen::OnLoginError(LoginErrorEvent& loginErrorEvent)
{
	// Show error.
	auto localizationComponent = this->game->entityManager->GetComponent<LocalizationComponent>(this->statusLabel, LocalizationComponent::LocalizationComponentType);
	localizationComponent->localizationKey = loginErrorEvent.errorMessage;

	auto localizedTextChangedEvent = std::make_shared<LocalizedTextChangedEvent>(this->statusLabel);
	this->game->eventManager->QueueEvent(localizedTextChangedEvent);

	// Add Reconnect button.
	this->connecting = false;

	this->reconnectButton = this->uiFactory->CreateSprite("Assets/Button.png");
	this->uiFactory->SetAnchor(this->reconnectButton, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.333f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->SetTappable(this->reconnectButton, true);
	this->uiFactory->FinishUIWidget(this->reconnectButton);

	this->reconnectLabel = this->uiFactory->CreateLabel(L"LoginScreen_Reconnect");
	this->uiFactory->SetAnchor(this->reconnectLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), this->reconnectButton);
	this->uiFactory->SetFontSize(this->reconnectLabel, 22.0f);
	this->uiFactory->FinishUIWidget(this->reconnectLabel);

	this->showReconnectUI = true;
}