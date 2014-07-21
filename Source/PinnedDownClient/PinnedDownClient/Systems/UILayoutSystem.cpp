#include "pch.h"
#include "Core\Event.h"
#include "Systems\UILayoutSystem.h"

#include "Components\UIPanelComponent.h"
#include "Components\BoundsComponent.h"
#include "Components\ScreenPositionComponent.h"

#include "Events\EntityInitializedEvent.h"

using namespace Windows::ApplicationModel;

using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Components;

UILayoutSystem::UILayoutSystem()
{
}

void UILayoutSystem::InitSystem(std::shared_ptr<PinnedDownClient::GameInfrastructure> game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(std::shared_ptr<IEventListener>(this), AppWindowChangedEvent::AppWindowChangedEventType);
	this->game->eventManager->AddListener(std::shared_ptr<IEventListener>(this), AppWindowSizeChangedEvent::AppWindowSizeChangedEventType);

	this->CreateRootPanel();
}

void UILayoutSystem::CreateRootPanel()
{
	this->rootPanelEntity = this->game->entityManager->CreateEntity();

	auto panelComponent = std::make_shared<UIPanelComponent>();
	this->game->entityManager->AddComponent(this->rootPanelEntity, panelComponent);

	auto boundsComponent = std::make_shared<BoundsComponent>();
	this->game->entityManager->AddComponent(this->rootPanelEntity, boundsComponent);

	auto screenPositionComponent = std::make_shared<ScreenPositionComponent>();
	this->game->entityManager->AddComponent(this->rootPanelEntity, screenPositionComponent);

	auto entityInitializedEvent = std::shared_ptr<Events::EntityInitializedEvent>(new Events::EntityInitializedEvent(this->rootPanelEntity));
	this->game->eventManager->QueueEvent(entityInitializedEvent);
}

void UILayoutSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == AppWindowChangedEvent::AppWindowChangedEventType)
	{
		AppWindowChangedEvent appWindowChangedEvent = static_cast<AppWindowChangedEvent&>(newEvent);
		this->OnAppWindowChanged(appWindowChangedEvent);
	}
	else if (newEvent.GetEventType() == AppWindowSizeChangedEvent::AppWindowSizeChangedEventType)
	{
		AppWindowSizeChangedEvent appWindowSizeChangedEvent = static_cast<AppWindowSizeChangedEvent&>(newEvent);
		this->OnAppWindowSizeChanged(appWindowSizeChangedEvent);
	}
}

void UILayoutSystem::OnAppWindowChanged(AppWindowChangedEvent appWindowChangedEvent)
{
	auto window = appWindowChangedEvent.appWindow;
	auto newSize = Vector2F(window->Bounds.Width, window->Bounds.Height);
	this->ResizeRootPanel(newSize);
}

void UILayoutSystem::OnAppWindowSizeChanged(AppWindowSizeChangedEvent appWindowSizeChangedEvent)
{
	auto newSize = appWindowSizeChangedEvent.size;
	this->ResizeRootPanel(newSize);
}

void UILayoutSystem::ResizeRootPanel(Vector2F newSize)
{
	auto boundsComponent = this->game->entityManager->GetComponent<BoundsComponent>(this->rootPanelEntity, BoundsComponent::BoundsComponentType);

	if (boundsComponent != nullptr)
	{
		boundsComponent->bounds = newSize;
	}
}