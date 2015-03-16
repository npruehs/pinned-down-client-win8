#include "pch.h"

#include "Actions\VerifyClientVersionAction.h"

#include "Events\LocalizedTextChangedEvent.h"

#include "Components\LocalizationComponent.h"
#include "Components\TextComponent.h"

#include "Resources\PinnedDownResourceManager.h"

#include "Source\Features\Network\Actions\ConnectToServerAction.h"

#include "Systems\Screens\LoginScreen.h"

#include "Util\VersionUtils.h"

using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Features::Network::Actions;
using namespace PinnedDownClient::Resources;
using namespace PinnedDownClient::Systems::Screens;


LoginScreen::LoginScreen()
	: loginStatus(LoginStatus::Default)
{
}

ScreenName LoginScreen::GetScreenName()
{
	return ScreenName::Login;
}


void LoginScreen::InitScreen(PinnedDownCore::Game* game, std::shared_ptr<ClientIdMapping> clientIdMapping, std::shared_ptr<EntityIdMapping> entityIdMapping)
{
	Screen::InitScreen(game, clientIdMapping, entityIdMapping);

	this->game->eventManager->AddListener(this, ClientVersionNotVerifiedEvent::ClientVersionNotVerifiedEventType);
	this->game->eventManager->AddListener(this, EntityTappedEvent::EntityTappedEventType);
	this->game->eventManager->AddListener(this, LoginErrorEvent::LoginErrorEventType);
	this->game->eventManager->AddListener(this, LoginSuccessEvent::LoginSuccessEventType);
}

void LoginScreen::DeInitScreen()
{
	this->game->eventManager->RemoveListener(this, ClientVersionNotVerifiedEvent::ClientVersionNotVerifiedEventType);
	this->game->eventManager->RemoveListener(this, EntityTappedEvent::EntityTappedEventType);
	this->game->eventManager->RemoveListener(this, LoginErrorEvent::LoginErrorEventType);
	this->game->eventManager->RemoveListener(this, LoginSuccessEvent::LoginSuccessEventType);
}

void LoginScreen::Update(float dt)
{
	this->totalTime += dt;

	if (this->loginStatus != LoginStatus::Connecting &&
		this->loginStatus != LoginStatus::VerifyingClientVersion)
	{
		return;
	}

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
	this->statusLabel = this->uiFactory->CreateLabel("LoginScreen_Connecting");
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

	if (this->loginStatus == LoginStatus::ConnectionError)
	{
		// Remove reconnect UI.
		this->game->entityManager->RemoveEntity(this->reconnectButton);
		this->game->entityManager->RemoveEntity(this->reconnectLabel);
	}
}

void LoginScreen::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == ClientVersionNotVerifiedEvent::ClientVersionNotVerifiedEventType)
	{
		auto clientVersionVerifiedEvent = static_cast<ClientVersionNotVerifiedEvent&>(newEvent);
		this->OnClientVersionNotVerified(clientVersionVerifiedEvent);
	}
	else if (newEvent.GetEventType() == EntityTappedEvent::EntityTappedEventType)
	{
		auto entityTappedEvent = static_cast<EntityTappedEvent&>(newEvent);
		this->OnEntityTapped(entityTappedEvent);
	}
	else if (newEvent.GetEventType() == LoginErrorEvent::LoginErrorEventType)
	{
		auto loginErrorEvent = static_cast<LoginErrorEvent&>(newEvent);
		this->OnLoginError(loginErrorEvent);
	}
	else if (newEvent.GetEventType() == LoginSuccessEvent::LoginSuccessEventType)
	{
		auto loginSuccessEvent = static_cast<LoginSuccessEvent&>(newEvent);
		this->OnLoginSuccess(loginSuccessEvent);
	}
}

void LoginScreen::DoLogin()
{
	// Hide reconnect button.
	if (this->loginStatus == LoginStatus::ConnectionError)
	{
		this->game->entityManager->RemoveEntity(this->reconnectButton);
		this->game->entityManager->RemoveEntity(this->reconnectLabel);
	}

	// Start connecting.
	auto connectToServerAction = std::make_shared<ConnectToServerAction>();
	this->game->eventManager->QueueEvent(connectToServerAction);

	this->loginStatus = LoginStatus::Connecting;

	// Set status label text.
	auto localizationComponent = this->game->entityManager->GetComponent<LocalizationComponent>(this->statusLabel, LocalizationComponent::LocalizationComponentType);
	localizationComponent->localizationKey = "LoginScreen_Connecting";

	auto localizedTextChangedEvent = std::make_shared<LocalizedTextChangedEvent>(this->statusLabel);
	this->game->eventManager->QueueEvent(localizedTextChangedEvent);
}

void LoginScreen::OnClientVersionNotVerified(ClientVersionNotVerifiedEvent& clientVersionNotVerifiedEvent)
{
	this->loginStatus = LoginStatus::ConnectionError;

	// Set status label text.
	auto localizationComponent = this->game->entityManager->GetComponent<LocalizationComponent>(this->statusLabel, LocalizationComponent::LocalizationComponentType);
	localizationComponent->localizationKey = "LoginScreen_ClientVersionOutOfDate";

	auto localizedTextChangedEvent = std::make_shared<LocalizedTextChangedEvent>(this->statusLabel);
	this->game->eventManager->QueueEvent(localizedTextChangedEvent);
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
	this->loginStatus = LoginStatus::ConnectionError;

	this->reconnectButton = this->uiFactory->CreateSprite("Assets/Button.png");
	this->uiFactory->SetAnchor(this->reconnectButton, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.333f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->SetTappable(this->reconnectButton, true);
	this->uiFactory->FinishUIWidget(this->reconnectButton);

	this->reconnectLabel = this->uiFactory->CreateLabel("LoginScreen_Reconnect");
	this->uiFactory->SetAnchor(this->reconnectLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), this->reconnectButton);
	this->uiFactory->SetFontSize(this->reconnectLabel, 22.0f);
	this->uiFactory->FinishUIWidget(this->reconnectLabel);
}

void LoginScreen::OnLoginSuccess(LoginSuccessEvent& loginSuccessEvent)
{
	this->loginStatus = LoginStatus::VerifyingClientVersion;

	// Send version number to server.
	auto major = GetPinnedDownClientVersionMajor();
	auto minor = GetPinnedDownClientVersionMinor();
	auto build = GetPinnedDownClientVersionBuild();
	auto revision = GetPinnedDownClientVersionRevision();

	auto verifyClientVersionAction = std::make_shared<VerifyClientVersionAction>(major, minor, build, revision);
	this->game->eventManager->QueueEvent(verifyClientVersionAction);

	// Set status label text.
	auto localizationComponent = this->game->entityManager->GetComponent<LocalizationComponent>(this->statusLabel, LocalizationComponent::LocalizationComponentType);
	localizationComponent->localizationKey = "LoginScreen_VerifyingClientVersion";

	auto localizedTextChangedEvent = std::make_shared<LocalizedTextChangedEvent>(this->statusLabel);
	this->game->eventManager->QueueEvent(localizedTextChangedEvent);
}