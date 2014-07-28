
#include "pch.h"
#include "Systems\Screens\GameScreen.h"

using namespace PinnedDownClient::Systems::Screens;

GameScreen::GameScreen()
{
}

ScreenName GameScreen::GetScreenName()
{
	return ScreenName::Game;
}

void GameScreen::LoadResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext)
{
	this->game->resourceManager->LoadBitmapFromFile(
		d2dContext.Get(),
		L"Assets/Window.png"
		);
	this->game->resourceManager->LoadBitmapFromFile(
		d2dContext.Get(),
		L"Assets/Button.png"
		);
}

void GameScreen::UnloadResources()
{
	this->game->resourceManager->UnloadResource(L"Assets/Window.png");
	this->game->resourceManager->UnloadResource(L"Assets/Button.png");
}

void GameScreen::LoadUI()
{
	// First panel.
	this->panel1 = this->uiFactory->CreatePanel();
	this->uiFactory->SetDepth(this->panel1, -100);
	this->uiFactory->SetVisible(this->panel1, false);
	this->uiFactory->FinishUIWidget(this->panel1);

	this->window1 = this->uiFactory->CreateSprite(L"Assets/Window.png");
	this->uiFactory->SetAnchor(this->window1, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->SetDepth(this->window1, 1);
	this->uiFactory->SetPanel(this->window1, this->panel1);
	this->uiFactory->FinishUIWidget(this->window1);

	this->button1 = this->uiFactory->CreateSprite(L"Assets/Button.png");
	this->uiFactory->SetAnchor(this->button1, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), this->window1);
	this->uiFactory->SetDepth(this->button1, 2);
	this->uiFactory->SetPanel(this->button1, this->panel1);
	this->uiFactory->SetTappable(this->button1);
	this->uiFactory->FinishUIWidget(this->button1);

	// Second panel.
	this->panel2 = this->uiFactory->CreatePanel();
	this->uiFactory->FinishUIWidget(this->panel2);

	this->window2 = this->uiFactory->CreateSprite(L"Assets/Window.png");
	this->uiFactory->SetAnchor(this->window2, VerticalAnchor(VerticalAnchorType::VerticalCenter, -100.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, -100.0f), 0);
	this->uiFactory->SetDepth(this->window2, 1);
	this->uiFactory->SetPanel(this->window2, this->panel2);
	this->uiFactory->FinishUIWidget(this->window2);

	this->button2 = this->uiFactory->CreateSprite(L"Assets/Button.png");
	this->uiFactory->SetAnchor(this->button2, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), this->window2);
	this->uiFactory->SetDepth(this->button2, 2);
	this->uiFactory->SetPanel(this->button2, this->panel2);
	this->uiFactory->SetTappable(this->button2);
	this->uiFactory->FinishUIWidget(this->button2);

	// Long label.
	this->longLabel = this->uiFactory->CreateLabel(L"This is a veeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeery very very long text!");
	this->uiFactory->SetAnchor(this->longLabel, VerticalAnchor(), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->FinishUIWidget(this->longLabel);
}

void GameScreen::UnloadUI()
{
	this->game->entityManager->RemoveEntity(this->panel1);
	this->game->entityManager->RemoveEntity(this->window1);
	this->game->entityManager->RemoveEntity(this->button1);
	this->game->entityManager->RemoveEntity(this->panel2);
	this->game->entityManager->RemoveEntity(this->window2);
	this->game->entityManager->RemoveEntity(this->button2);
	this->game->entityManager->RemoveEntity(this->longLabel);
}