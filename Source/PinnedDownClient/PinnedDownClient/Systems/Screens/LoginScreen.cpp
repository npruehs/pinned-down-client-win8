#include "pch.h"

#include "Core\PinnedDownResourceManager.h"

#include "Components\TextComponent.h"

#include "Events\LoginSuccessEvent.h"
#include "Events\LoginErrorEvent.h"

#include "Systems\Screens\LoginScreen.h"


using namespace PinnedDownClient::Core;
using namespace PinnedDownClient::Events;
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

	this->game->eventManager->AddListener(this, LoginErrorEvent::LoginErrorEventType);

	this->connecting = true;
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
	if (newEvent.GetEventType() == LoginErrorEvent::LoginErrorEventType)
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
}

void LoginScreen::UnloadResources()
{
	this->game->resourceManager->UnloadResource(L"Assets/SplashScreen.png");
}

void LoginScreen::LoadUI()
{
	this->statusLabel = this->uiFactory->CreateLabel(L"Connecting...");
	this->uiFactory->SetAnchor(this->statusLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 200.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->SetFontSize(this->statusLabel, 18.0f);
	this->uiFactory->FinishUIWidget(this->statusLabel);

	this->splashScreen = this->uiFactory->CreateSprite(L"Assets/SplashScreen.png");
	this->uiFactory->SetAnchor(this->splashScreen, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->FinishUIWidget(this->splashScreen);
}

void LoginScreen::UnloadUI()
{
	this->game->entityManager->RemoveEntity(this->statusLabel);
}

void LoginScreen::OnLoginError(LoginErrorEvent& loginErrorEvent)
{
	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->statusLabel, TextComponent::TextComponentType);
	textComponent->text = loginErrorEvent.errorMessage;

	this->connecting = false;
}