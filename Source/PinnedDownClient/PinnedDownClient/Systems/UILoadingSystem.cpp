
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
		L"Assets/Logo.png"
		);

	this->game->resourceManager->LoadBitmapFromFile(
		d2dContext.Get(),
		L"Assets/SmallLogo.png"
		);

	this->game->resourceManager->LoadBitmapFromFile(
		d2dContext.Get(),
		L"Assets/SplashScreen.png"
		);

	this->game->resourceManager->LoadBitmapFromFile(
		d2dContext.Get(),
		L"Assets/StoreLogo.png"
		);
}

void UILoadingSystem::UnloadResources()
{
	this->game->resourceManager->UnloadResource(L"Assets/Logo.png");
	this->game->resourceManager->UnloadResource(L"Assets/SmallLogo.png");
	this->game->resourceManager->UnloadResource(L"Assets/SplashScreen.png");
	this->game->resourceManager->UnloadResource(L"Assets/StoreLogo.png");
}

void UILoadingSystem::LoadUI()
{
	int sprite = this->uiFactory->CreateSprite(L"Assets/Logo.png", VerticalAnchor(), HorizontalAnchor(HorizontalAnchorType::Left, 50.0f));
	int label = this->uiFactory->CreateLabel(VerticalAnchor(), HorizontalAnchor(), sprite, L"Anchored Text");
}
