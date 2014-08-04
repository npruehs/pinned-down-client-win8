#include "pch.h"
#include "Event.h"
#include "Systems\UILayoutSystem.h"

#include "Components\UIPanelComponent.h"
#include "Components\BoundsComponent.h"
#include "Components\ScreenPositionComponent.h"

using namespace PinnedDownCore;

using namespace Windows::ApplicationModel;

using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Components;

UILayoutSystem::UILayoutSystem()
{
}

void UILayoutSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	this->uiFactory = std::make_shared<UIFactory>(game);

	this->game->eventManager->AddListener(this, AppWindowChangedEvent::AppWindowChangedEventType);
	this->game->eventManager->AddListener(this, AppWindowSizeChangedEvent::AppWindowSizeChangedEventType);
	this->game->eventManager->AddListener(this, EntityInitializedEvent::EntityInitializedEventType);
	this->game->eventManager->AddListener(this, EntityRemovedEvent::EntityRemovedEventType);

	this->CreateRootPanel();
}

void UILayoutSystem::CreateRootPanel()
{
	// Create panel entity.
	int entityId = this->uiFactory->CreatePanel();
	this->uiFactory->FinishUIWidget(entityId);

	// Set root panel.
	Panel panel = Panel();
	panel.entityId = entityId;
	panel.boundsComponent = this->game->entityManager->GetComponent<BoundsComponent>(entityId, BoundsComponent::BoundsComponentType);
	panel.screenPositionComponent = this->game->entityManager->GetComponent<ScreenPositionComponent>(entityId, ScreenPositionComponent::ScreenPositionComponentType);
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
	else if (newEvent.GetEventType() == EntityRemovedEvent::EntityRemovedEventType)
	{
		EntityRemovedEvent entityRemovedEvent = static_cast<EntityRemovedEvent&>(newEvent);
		this->OnEntityRemoved(entityRemovedEvent);
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
	auto boundsComponent = this->game->entityManager->GetComponent<BoundsComponent>(entityId, BoundsComponent::BoundsComponentType);
	auto screenPositionComponent = this->game->entityManager->GetComponent<ScreenPositionComponent>(entityId, ScreenPositionComponent::ScreenPositionComponentType);

	if (anchorComponent != nullptr
		&& boundsComponent != nullptr
		&& screenPositionComponent != nullptr)
	{
		// Add anchor.
		Anchor anchor = Anchor();
		anchor.entityId = entityId;
		anchor.anchorComponent = anchorComponent;
		anchor.boundsComponent = boundsComponent;
		anchor.screenPositionComponent = screenPositionComponent;

		this->anchors.push_back(anchor);
	}
}

void UILayoutSystem::OnEntityRemoved(EntityRemovedEvent entityRemovedEvent)
{
	// Remove anchor.
	for (std::list<Anchor>::iterator iterator = this->anchors.begin(); iterator != this->anchors.end(); ++iterator)
	{
		Anchor& anchor = *iterator;

		if (anchor.entityId == entityRemovedEvent.entityId)
		{
			this->anchors.erase(iterator);
			return;
		}
	}
}

void UILayoutSystem::Update(float dt)
{
	// Update anchors.
	for (std::list<Anchor>::iterator iterator = this->anchors.begin(); iterator != this->anchors.end(); iterator++)
	{
		Anchor& anchor = *iterator;

		HorizontalAnchor& left = anchor.anchorComponent->left;
		VerticalAnchor& top = anchor.anchorComponent->top;

		auto x = 0.0f;
		auto y = 0.0f;

		// Check for anchor target.
		std::shared_ptr<ScreenPositionComponent> targetPosition;
		std::shared_ptr<BoundsComponent> targetBounds;

		if (anchor.anchorComponent->target != INVALID_ENTITY_ID)
		{
			targetPosition = this->game->entityManager->GetComponent<ScreenPositionComponent>(anchor.anchorComponent->target, ScreenPositionComponent::ScreenPositionComponentType);
			targetBounds = this->game->entityManager->GetComponent<BoundsComponent>(anchor.anchorComponent->target, BoundsComponent::BoundsComponentType);
		}
		else
		{
			targetPosition = this->rootPanel.screenPositionComponent;
			targetBounds = this->rootPanel.boundsComponent;
		}

		// Consider position.
		x += targetPosition->position.x;
		y += targetPosition->position.y;

		// Consider bounds.
		if (left.type == HorizontalAnchorType::Right)
		{
			x += targetBounds->bounds.x - anchor.boundsComponent->bounds.x;
		}
		else if (left.type == HorizontalAnchorType::HorizontalCenter)
		{
			x += (targetBounds->bounds.x - anchor.boundsComponent->bounds.x) / 2;
		}

		if (top.type == VerticalAnchorType::Bottom)
		{
			y += targetBounds->bounds.y - anchor.boundsComponent->bounds.y;
		}
		else if (top.type == VerticalAnchorType::VerticalCenter)
		{
			y += (targetBounds->bounds.y - anchor.boundsComponent->bounds.y) / 2;
		}

		// Add relative offset.
		x += left.offset;
		y += top.offset;

		// Update screen position.
		anchor.screenPositionComponent->position = Vector2F(x, y);
	}
}

void UILayoutSystem::ResizeRootPanel(Vector2F newSize)
{
	this->rootPanel.boundsComponent->bounds = newSize;
}