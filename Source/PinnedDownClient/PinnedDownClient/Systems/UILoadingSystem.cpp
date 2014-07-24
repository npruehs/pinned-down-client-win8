
#include "pch.h"
#include "Core\Event.h"
#include "Systems\UILoadingSystem.h"

using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Systems::UI;

UILoadingSystem::UILoadingSystem()
{
}

void UILoadingSystem::InitSystem(std::shared_ptr<PinnedDownClient::GameInfrastructure> game)
{
	GameSystem::InitSystem(game);

	this->uiFactory = std::make_shared<UIFactory>(game);

	this->game->eventManager->AddListener(std::shared_ptr<IEventListener>(this), RenderTargetChangedEvent::RenderTargetChangedEventType);
}

void UILoadingSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == RenderTargetChangedEvent::RenderTargetChangedEventType)
	{
		RenderTargetChangedEvent renderTargetChangedEvent = static_cast<RenderTargetChangedEvent&>(newEvent);
		this->OnRenderTargetChanged(renderTargetChangedEvent);
	}
}

void UILoadingSystem::OnRenderTargetChanged(RenderTargetChangedEvent renderTargetChangedEvent)
{
	this->UnloadResources();
	this->LoadResources(renderTargetChangedEvent.d2dContext);

	this->LoadUI();
}

void UILoadingSystem::LoadResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext)
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

void UILoadingSystem::UnloadResources()
{
	this->game->resourceManager->UnloadResource(L"Assets/Window.png");
	this->game->resourceManager->UnloadResource(L"Assets/Button.png");
}

void UILoadingSystem::LoadUI()
{
	// First panel.
	int panel = this->uiFactory->CreatePanel();
	this->uiFactory->SetDepth(panel, -100);
	this->uiFactory->FinishUIWidget(panel);

	int window = this->uiFactory->CreateSprite(L"Assets/Window.png");
	this->uiFactory->SetAnchor(window, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->SetDepth(window, 1);
	this->uiFactory->SetPanel(window, panel);
	this->uiFactory->FinishUIWidget(window);

	int button = this->uiFactory->CreateSprite(L"Assets/Button.png");
	this->uiFactory->SetAnchor(button, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), window);
	this->uiFactory->SetDepth(button, 2);
	this->uiFactory->SetPanel(button, panel);
	this->uiFactory->FinishUIWidget(button);

	// Second panel.
	panel = this->uiFactory->CreatePanel();
	this->uiFactory->FinishUIWidget(panel);

	window = this->uiFactory->CreateSprite(L"Assets/Window.png");
	this->uiFactory->SetAnchor(window, VerticalAnchor(VerticalAnchorType::VerticalCenter, -100.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, -100.0f), 0);
	this->uiFactory->SetDepth(window, 1);
	this->uiFactory->SetPanel(window, panel);
	this->uiFactory->FinishUIWidget(window);

	button = this->uiFactory->CreateSprite(L"Assets/Button.png");
	this->uiFactory->SetAnchor(button, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), window);
	this->uiFactory->SetDepth(button, 2);
	this->uiFactory->SetPanel(button, panel);
	this->uiFactory->FinishUIWidget(button);

	// Long label.
	int longLabel = this->uiFactory->CreateLabel(L"This is a veeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeery very very long text!");
	this->uiFactory->SetAnchor(longLabel, VerticalAnchor(), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->FinishUIWidget(longLabel);
}
