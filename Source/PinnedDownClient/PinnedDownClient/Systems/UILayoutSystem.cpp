#include "pch.h"
#include "Core\Event.h"
#include "Systems\UILayoutSystem.h"

#include "Components\UIPanelComponent.h"
#include "Components\BoundsComponent.h"
#include "Components\ScreenPositionComponent.h"

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
	this->game->eventManager->AddListener(std::shared_ptr<IEventListener>(this), EntityInitializedEvent::EntityInitializedEventType);

	this->CreateRootPanel();
}

void UILayoutSystem::CreateRootPanel()
{
	// Create panel entity.
	int rootPanelEntity = this->game->entityManager->CreateEntity();

	auto panelComponent = std::make_shared<UIPanelComponent>();
	this->game->entityManager->AddComponent(rootPanelEntity, panelComponent);

	auto boundsComponent = std::make_shared<BoundsComponent>();
	this->game->entityManager->AddComponent(rootPanelEntity, boundsComponent);

	auto screenPositionComponent = std::make_shared<ScreenPositionComponent>();
	this->game->entityManager->AddComponent(rootPanelEntity, screenPositionComponent);

	auto entityInitializedEvent = std::shared_ptr<Events::EntityInitializedEvent>(new Events::EntityInitializedEvent(rootPanelEntity));
	this->game->eventManager->QueueEvent(entityInitializedEvent);

	// Add panel.
	Panel panel = Panel();
	panel.entityId = rootPanelEntity;
	panel.boundsComponent = boundsComponent;

	this->rootPanel = panel;
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
	else if (newEvent.GetEventType() == EntityInitializedEvent::EntityInitializedEventType)
	{
		EntityInitializedEvent entityInitializedEvent = static_cast<EntityInitializedEvent&>(newEvent);
		this->OnEntityInitialized(entityInitializedEvent);
	}
}

void UILayoutSystem::OnAppWindowChanged(AppWindowChangedEvent appWindowChangedEvent)
{
	// Resize root panel.
	auto window = appWindowChangedEvent.appWindow;
	auto newSize = Vector2F(window->Bounds.Width, window->Bounds.Height);
	this->ResizeRootPanel(newSize);
}

void UILayoutSystem::OnAppWindowSizeChanged(AppWindowSizeChangedEvent appWindowSizeChangedEvent)
{
	// Resize root panel.
	auto newSize = appWindowSizeChangedEvent.size;
	this->ResizeRootPanel(newSize);
}

void UILayoutSystem::OnEntityInitialized(EntityInitializedEvent entityInitializedEvent)
{
	auto entityId = entityInitializedEvent.entityId;
	auto anchorComponent = this->game->entityManager->GetComponent<UIAnchorComponent>(entityId, UIAnchorComponent::UIAnchorComponentType);
	auto screenPositionComponent = this->game->entityManager->GetComponent<ScreenPositionComponent>(entityId, ScreenPositionComponent::ScreenPositionComponentType);

	if (anchorComponent != nullptr
		&& screenPositionComponent != nullptr)
	{
		// Add anchor.
		Anchor anchor = Anchor();
		anchor.entityId = entityId;
		anchor.anchorComponent = anchorComponent;
		anchor.screenPositionComponent = screenPositionComponent;

		this->anchors.push_back(anchor);
	}
}

void UILayoutSystem::Update(StepTimer const& timer)
{
	auto x = 0.0f;
	auto y = 0.0f;

	// Update anchors.
	for (std::list<Anchor>::iterator iterator = this->anchors.begin(); iterator != this->anchors.end(); iterator++)
	{
		Anchor& anchor = *iterator;

		HorizontalAnchor& left = anchor.anchorComponent->left;
		VerticalAnchor& top = anchor.anchorComponent->top;

		x = left.offset;
		y = top.offset;

		if (left.type == HorizontalAnchorType::Right)
		{
			x += this->rootPanel.boundsComponent->bounds.x;
		}

		if (top.type == VerticalAnchorType::Bottom)
		{
			y += this->rootPanel.boundsComponent->bounds.y;
		}

		anchor.screenPositionComponent->position = Vector2F(x, y);
	}
}

void UILayoutSystem::ResizeRootPanel(Vector2F newSize)
{
	this->rootPanel.boundsComponent->bounds = newSize;
}