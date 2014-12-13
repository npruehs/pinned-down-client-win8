#include "pch.h"
#include "Event.h"
#include "Systems\UIInteractionSystem.h"

#include "Components\BoundsComponent.h"
#include "Components\ScreenPositionComponent.h"
#include "Components\TappableComponent.h"

#include "EntityRemovedEvent.h"
#include "Events\EntityHoveredEvent.h"
#include "Events\EntityTappedEvent.h"
#include "Events\EntityUnhoveredEvent.h"

#include "Math\RectF.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Components;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Math;
using namespace PinnedDownClient::Systems;


UIInteractionSystem::UIInteractionSystem()
{
}

void UIInteractionSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, EntityInitializedEvent::EntityInitializedEventType);
	this->game->eventManager->AddListener(this, EntityRemovedEvent::EntityRemovedEventType);
	this->game->eventManager->AddListener(this, PointerPressedEvent::PointerPressedEventType);
	this->game->eventManager->AddListener(this, PointerReleasedEvent::PointerReleasedEventType);
}

void UIInteractionSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(EntityInitializedEvent);
	CALL_EVENT_HANDLER(EntityRemovedEvent);
	CALL_EVENT_HANDLER(PointerPressedEvent);
	CALL_EVENT_HANDLER(PointerReleasedEvent);
}

void UIInteractionSystem::Update(float dt)
{
	if (this->currentButton == nullptr)
	{
		return;
	}

	auto oldElapsedTapTime = this->elapsedTapTime;
	auto newElapedTapTime = oldElapsedTapTime + dt;

	if (oldElapsedTapTime < this->hoverThreshold && newElapedTapTime >= this->hoverThreshold)
	{
		// Notify listeners.
		auto entityHoveredEvent = std::make_shared<EntityHoveredEvent>(this->currentButton->entity);
		this->game->eventManager->QueueEvent(entityHoveredEvent);
	}

	this->elapsedTapTime = newElapedTapTime;
}

EVENT_HANDLER_DEFINITION(UIInteractionSystem, EntityInitializedEvent)
{
	auto boundsComponent = this->game->entityManager->GetComponent<BoundsComponent>(data.entity, BoundsComponent::BoundsComponentType);
	auto depthComponent = this->game->entityManager->GetComponent<DepthComponent>(data.entity, DepthComponent::DepthComponentType);
	auto positionComponent = this->game->entityManager->GetComponent<ScreenPositionComponent>(data.entity, ScreenPositionComponent::ScreenPositionComponentType);
	auto tappableComponent = this->game->entityManager->GetComponent<TappableComponent>(data.entity, TappableComponent::TappableComponentType);

	if (boundsComponent != nullptr
		&& depthComponent != nullptr
		&& positionComponent != nullptr
		&& tappableComponent != nullptr)
	{
		auto button = UI::Button();
		button.entity = data.entity;
		button.boundsComponent = boundsComponent;
		button.depthComponent = depthComponent;
		button.screenPositionComponent = positionComponent;
		button.tappableComponent = tappableComponent;

		this->buttons.push_back(button);
	}
}

EVENT_HANDLER_DEFINITION(UIInteractionSystem, EntityRemovedEvent)
{
	for (std::list<UI::Button>::iterator iterator = this->buttons.begin(); iterator != this->buttons.end(); ++iterator)
	{
		UI::Button& button = *iterator;

		if (button.entity == data.entity)
		{
			this->buttons.erase(iterator);
			return;
		}
	}
}

EVENT_HANDLER_DEFINITION(UIInteractionSystem, PointerPressedEvent)
{
	this->elapsedTapTime = 0.0f;

	// Hit testing.
	this->currentButton = this->HitTest(data.position);
}

EVENT_HANDLER_DEFINITION(UIInteractionSystem, PointerReleasedEvent)
{
	if (this->currentButton == nullptr)
	{
		return;
	}

	UI::Button* tappedButton = this->HitTest(data.position);

	if (this->currentButton != tappedButton)
	{
		return;
	}

	// Notify listeners.
	if (this->elapsedTapTime < this->hoverThreshold)
	{
		auto entityTappedEvent = std::make_shared<EntityTappedEvent>(this->currentButton->entity);
		this->game->eventManager->QueueEvent(entityTappedEvent);
	}
	else
	{
		auto entityUnhoveredEvent = std::make_shared<EntityUnhoveredEvent>(this->currentButton->entity);
		this->game->eventManager->QueueEvent(entityUnhoveredEvent);
	}

	// Reset state.
	this->currentButton = nullptr;
}

UI::Button* UIInteractionSystem::HitTest(Vector2F position)
{
	UI::Button* tappedButton = nullptr;

	for (std::list<UI::Button>::iterator iterator = this->buttons.begin(); iterator != this->buttons.end(); ++iterator)
	{
		UI::Button& button = *iterator;

		if (button.tappableComponent->tappable)
		{
			RectF buttonRect = RectF(button.screenPositionComponent->position, button.boundsComponent->bounds);

			if (buttonRect.Contains(position))
			{
				// Button hit, check depth.
				if (tappedButton == nullptr || (tappedButton->depthComponent->depth < button.depthComponent->depth))
				{
					tappedButton = &button;
				}
			}
		}
	}

	return tappedButton;
}