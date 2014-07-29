#include "pch.h"
#include "Core\Event.h"
#include "Systems\UIInteractionSystem.h"

#include "Components\BoundsComponent.h"
#include "Components\ScreenPositionComponent.h"
#include "Components\TappableComponent.h"

#include "Events\EntityTappedEvent.h"

#include "Math\RectF.h"


using namespace PinnedDownClient::Components;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Math;
using namespace PinnedDownClient::Systems;


UIInteractionSystem::UIInteractionSystem()
{
}

void UIInteractionSystem::InitSystem(std::shared_ptr<PinnedDownClient::GameInfrastructure> game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, EntityInitializedEvent::EntityInitializedEventType);
	this->game->eventManager->AddListener(this, EntityRemovedEvent::EntityRemovedEventType);
	this->game->eventManager->AddListener(this, PointerReleasedEvent::PointerReleasedEventType);
}

void UIInteractionSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == EntityInitializedEvent::EntityInitializedEventType)
	{
		EntityInitializedEvent entityInitializedEvent = static_cast<EntityInitializedEvent&>(newEvent);
		this->OnEntityInitialized(entityInitializedEvent);
	}
	else if (newEvent.GetEventType() == EntityRemovedEvent::EntityRemovedEventType)
	{
		EntityRemovedEvent entityRemovedEvent = static_cast<EntityRemovedEvent&>(newEvent);
		this->OnEntityRemoved(entityRemovedEvent);
	}
	else if (newEvent.GetEventType() == PointerReleasedEvent::PointerReleasedEventType)
	{
		PointerReleasedEvent pointerReleasedEvent = static_cast<PointerReleasedEvent&>(newEvent);
		this->OnPointerReleased(pointerReleasedEvent);
	}
}

void UIInteractionSystem::OnEntityInitialized(EntityInitializedEvent entityInitializedEvent)
{
	auto boundsComponent = this->game->entityManager->GetComponent<BoundsComponent>(entityInitializedEvent.entityId, BoundsComponent::BoundsComponentType);
	auto depthComponent = this->game->entityManager->GetComponent<DepthComponent>(entityInitializedEvent.entityId, DepthComponent::DepthComponentType);
	auto positionComponent = this->game->entityManager->GetComponent<ScreenPositionComponent>(entityInitializedEvent.entityId, ScreenPositionComponent::ScreenPositionComponentType);
	auto tappableComponent = this->game->entityManager->GetComponent<TappableComponent>(entityInitializedEvent.entityId, TappableComponent::TappableComponentType);

	if (boundsComponent != nullptr
		&& depthComponent != nullptr
		&& positionComponent != nullptr
		&& tappableComponent != nullptr)
	{
		auto button = UI::Button();
		button.entityId = entityInitializedEvent.entityId;
		button.boundsComponent = boundsComponent;
		button.depthComponent = depthComponent;
		button.screenPositionComponent = positionComponent;

		this->buttons.push_back(button);
	}
}

void UIInteractionSystem::OnEntityRemoved(EntityRemovedEvent entityRemovedEvent)
{
	for (std::list<UI::Button>::iterator iterator = this->buttons.begin(); iterator != this->buttons.end(); ++iterator)
	{
		UI::Button& button = *iterator;

		if (button.entityId == entityRemovedEvent.entityId)
		{
			this->buttons.erase(iterator);
			return;
		}
	}
}

void UIInteractionSystem::OnPointerReleased(PointerReleasedEvent pointerReleasedEvent)
{
	// Hit testing.
	UI::Button* tappedButton = nullptr;

	for (std::list<UI::Button>::iterator iterator = this->buttons.begin(); iterator != this->buttons.end(); ++iterator)
	{
		UI::Button& button = *iterator;
		RectF buttonRect = RectF(button.screenPositionComponent->position, button.boundsComponent->bounds);

		if (buttonRect.Contains(pointerReleasedEvent.position))
		{
			// Button hit, check depth.
			if (tappedButton == nullptr || (tappedButton->depthComponent->depth < button.depthComponent->depth))
			{
				tappedButton = &button;
			}
		}
	}

	if (tappedButton != nullptr)
	{
		// Notify listeners.
		auto entityTappedEvent = std::make_shared<EntityTappedEvent>(tappedButton->entityId);
		this->game->eventManager->QueueEvent(entityTappedEvent);
	}
}