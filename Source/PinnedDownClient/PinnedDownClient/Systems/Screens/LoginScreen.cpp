
#include "pch.h"
#include "Systems\Screens\LoginScreen.h"

using namespace PinnedDownClient::Systems::Screens;

LoginScreen::LoginScreen()
{
}

ScreenName LoginScreen::GetScreenName()
{
	return ScreenName::Login;
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