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

	this->game->eventManager->AddListener(std::shared_ptr<IEventListener>(this), LoginSuccessEvent::LoginSuccessEventType);
	this->game->eventManager->AddListener(std::shared_ptr<IEventListener>(this), LoginErrorEvent::LoginErrorEventType);
}

void LoginScreen::DeInitScreen()
{
	this->game->eventManager->RemoveListener(std::shared_ptr<IEventListener>(this), LoginSuccessEvent::LoginSuccessEventType);
	this->game->eventManager->RemoveListener(std::shared_ptr<IEventListener>(this), LoginErrorEvent::LoginErrorEventType);
}

void LoginScreen::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == LoginSuccessEvent::LoginSuccessEventType)
	{
		this->OnLoginSuccess();
	}
	else if (newEvent.GetEventType() == LoginErrorEvent::LoginErrorEventType)
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

void LoginScreen::OnLoginSuccess()
{
}

void LoginScreen::OnLoginError(LoginErrorEvent& loginErrorEvent)
{
	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->statusLabel, TextComponent::TextComponentType);
	textComponent->text = loginErrorEvent.errorMessage;
}