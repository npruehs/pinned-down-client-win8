#include "pch.h"
#include "Event.h"
#include "Systems\UILayoutSystem.h"

#include "Source\Features\Layout\Components\BoundsComponent.h"
#include "Source\Features\Layout\Components\ScreenPositionComponent.h"
#include "Source\Features\UI\Components\UIPanelComponent.h"


using namespace PinnedDownCore;

using namespace Windows::ApplicationModel;

using namespace PinnedDownClient::Features::Layout;
using namespace PinnedDownClient::Features::UI;
using namespace PinnedDownClient::Systems;


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
	Entity entity = this->uiFactory->CreatePanel();
	this->uiFactory->FinishUIWidget(entity);

	// Set root panel.
	Panel panel = Panel();
	panel.entity = entity;
	panel.boundsComponent = this->game->entityManager->GetComponent<BoundsComponent>(entity, BoundsComponent::BoundsComponentType);
	panel.screenPositionComponent = this->game->entityManager->GetComponent<ScreenPositionComponent>(entity, ScreenPositionComponent::ScreenPositionComponentType);
	this->rootPanel = panel;
}

void UILayoutSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(AppWindowChangedEvent);
	CALL_EVENT_HANDLER(AppWindowSizeChangedEvent);
	CALL_EVENT_HANDLER(EntityInitializedEvent);
	CALL_EVENT_HANDLER(EntityRemovedEvent);
}

EVENT_HANDLER_DEFINITION(UILayoutSystem, AppWindowChangedEvent)
{
	// Resize root panel.
	auto window = data.appWindow;
	auto newSize = Vector2F(window->Bounds.Width, window->Bounds.Height);
	this->ResizeRootPanel(newSize);
}

EVENT_HANDLER_DEFINITION(UILayoutSystem, AppWindowSizeChangedEvent)
{
	// Resize root panel.
	auto newSize = data.size;
	this->ResizeRootPanel(newSize);
}

EVENT_HANDLER_DEFINITION(UILayoutSystem, EntityInitializedEvent)
{
	auto entity = data.entity;
	auto anchorComponent = this->game->entityManager->GetComponent<UIAnchorComponent>(entity, UIAnchorComponent::UIAnchorComponentType);
	auto boundsComponent = this->game->entityManager->GetComponent<BoundsComponent>(entity, BoundsComponent::BoundsComponentType);
	auto screenPositionComponent = this->game->entityManager->GetComponent<ScreenPositionComponent>(entity, ScreenPositionComponent::ScreenPositionComponentType);

	if (anchorComponent != nullptr
		&& boundsComponent != nullptr
		&& screenPositionComponent != nullptr)
	{
		// Add anchor.
		Anchor anchor = Anchor();
		anchor.entity = entity;
		anchor.anchorComponent = anchorComponent;
		anchor.boundsComponent = boundsComponent;
		anchor.screenPositionComponent = screenPositionComponent;

		this->anchors.push_back(anchor);
	}
}

EVENT_HANDLER_DEFINITION(UILayoutSystem, EntityRemovedEvent)
{
	for (std::list<Anchor>::iterator iterator = this->anchors.begin(); iterator != this->anchors.end(); )
	{
		Anchor& anchor = *iterator;

		if (anchor.anchorComponent->target == data.entity)
		{
			// Reset anchor.
			anchor.anchorComponent->target = INVALID_ENTITY_ID;
			anchor.screenPositionComponent->positionValid = false;
		}

		if (anchor.entity == data.entity)
		{
			// Remove anchor.
			iterator = this->anchors.erase(iterator);
		}
		else
		{
			++iterator;
		}
	}
}

void UILayoutSystem::Update(float dt)
{
	// Update anchors.
	for (std::list<Anchor>::iterator iterator = this->anchors.begin(); iterator != this->anchors.end(); ++iterator)
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
		x += this->rootPanel.boundsComponent->bounds.x * left.relativeOffset + left.absoluteOffset;
		y += this->rootPanel.boundsComponent->bounds.y * top.relativeOffset  + top.absoluteOffset;

		// Update screen position.
		anchor.screenPositionComponent->position = Vector2F(x, y);
		anchor.screenPositionComponent->positionValid = true;
	}
}

void UILayoutSystem::ResizeRootPanel(Vector2F newSize)
{
	this->rootPanel.boundsComponent->bounds = newSize;
}