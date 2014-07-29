#include "pch.h"

#include "Components\TextComponent.h"

#include "Events\LoginSuccessEvent.h"
#include "Events\LoginErrorEvent.h"

#include "Systems\Screens\LoginScreen.h"


using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems::Screens;


LoginScreen::LoginScreen()
{
}

ScreenName LoginScreen::GetScreenName()
{
	return ScreenName::Login;
}


void LoginScreen::InitScreen(std::shared_ptr<PinnedDownClient::GameInfrastructure> game)
{
	Screen::InitScreen(game);

	this->game->eventManager->AddListener(this, LoginErrorEvent::LoginErrorEventType);

	this->connecting = true;
}

void LoginScreen::DeInitScreen()
{
	this->game->eventManager->RemoveListener(this, LoginErrorEvent::LoginErrorEventType);
}

void LoginScreen::Update()
{
	if (!this->connecting)
	{
		return;
	}

	std::wstring text = L"Connecting ";

	if (this->game->timer->GetFrameCount() % 60 < 20)
	{
		text += L".";
	}
	else if (this->game->timer->GetFrameCount() % 60 < 40)
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

void LoginScreen::LoadUI()
{
	this->statusLabel = this->uiFactory->CreateLabel(L"Connecting...");
	this->uiFactory->SetAnchor(this->statusLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->FinishUIWidget(this->statusLabel);
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