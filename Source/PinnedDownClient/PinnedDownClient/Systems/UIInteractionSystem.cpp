#include "pch.h"
#include "Event.h"
#include "Systems\UIInteractionSystem.h"

#include "Actions\PlaySoundAction.h"

#include "Components\BoundsComponent.h"
#include "Components\ScreenPositionComponent.h"
#include "Components\TappableComponent.h"

#include "EntityRemovedEvent.h"
#include "Events\EntityHoveredEvent.h"
#include "Events\EntityTappedEvent.h"
#include "Events\EntityUnhoveredEvent.h"

#include "Math\RectF.h"

#include "Resources\PinnedDownResourceManager.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Components;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Math;
using namespace PinnedDownClient::Resources;
using namespace PinnedDownClient::Systems;


UIInteractionSystem::UIInteractionSystem()
{
}

void UIInteractionSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, AudioEngineChangedEvent::AudioEngineChangedEventType);
	this->game->eventManager->AddListener(this, EntityInitializedEvent::EntityInitializedEventType);
	this->game->eventManager->AddListener(this, EntityRemovedEvent::EntityRemovedEventType);
	this->game->eventManager->AddListener(this, PointerPressedEvent::PointerPressedEventType);
	this->game->eventManager->AddListener(this, PointerReleasedEvent::PointerReleasedEventType);

	this->LoadResources();
}

void UIInteractionSystem::LoadResources()
{
	auto resourceManager = static_cast<PinnedDownResourceManager*>(this->game->resourceManager.get());

	resourceManager->LoadAudioFromFile(
		this->soundAudioEngine.Get(),
		L"Assets/sfx_button.wav"
		);
}

void UIInteractionSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == AudioEngineChangedEvent::AudioEngineChangedEventType)
	{
		AudioEngineChangedEvent audioEngineChangedEvent = static_cast<AudioEngineChangedEvent&>(newEvent);
		this->OnAudioEngineChanged(audioEngineChangedEvent);
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
	else if (newEvent.GetEventType() == PointerPressedEvent::PointerPressedEventType)
	{
		PointerPressedEvent pointerPressedEvent = static_cast<PointerPressedEvent&>(newEvent);
		this->OnPointerPressed(pointerPressedEvent);
	}
	else if (newEvent.GetEventType() == PointerReleasedEvent::PointerReleasedEventType)
	{
		PointerReleasedEvent pointerReleasedEvent = static_cast<PointerReleasedEvent&>(newEvent);
		this->OnPointerReleased(pointerReleasedEvent);
	}
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

void UIInteractionSystem::OnAudioEngineChanged(AudioEngineChangedEvent& audioEngineChangedEvent)
{
	this->soundAudioEngine = audioEngineChangedEvent.audioEngine;
}

void UIInteractionSystem::OnEntityInitialized(EntityInitializedEvent& entityInitializedEvent)
{
	auto boundsComponent = this->game->entityManager->GetComponent<BoundsComponent>(entityInitializedEvent.entity, BoundsComponent::BoundsComponentType);
	auto depthComponent = this->game->entityManager->GetComponent<DepthComponent>(entityInitializedEvent.entity, DepthComponent::DepthComponentType);
	auto positionComponent = this->game->entityManager->GetComponent<ScreenPositionComponent>(entityInitializedEvent.entity, ScreenPositionComponent::ScreenPositionComponentType);
	auto tappableComponent = this->game->entityManager->GetComponent<TappableComponent>(entityInitializedEvent.entity, TappableComponent::TappableComponentType);

	if (boundsComponent != nullptr
		&& depthComponent != nullptr
		&& positionComponent != nullptr
		&& tappableComponent != nullptr)
	{
		auto button = UI::Button();
		button.entity = entityInitializedEvent.entity;
		button.boundsComponent = boundsComponent;
		button.depthComponent = depthComponent;
		button.screenPositionComponent = positionComponent;

		this->buttons.push_back(button);
	}
}

void UIInteractionSystem::OnEntityRemoved(EntityRemovedEvent& entityRemovedEvent)
{
	for (std::list<UI::Button>::iterator iterator = this->buttons.begin(); iterator != this->buttons.end(); ++iterator)
	{
		UI::Button& button = *iterator;

		if (button.entity == entityRemovedEvent.entity)
		{
			this->buttons.erase(iterator);
			return;
		}
	}
}

void UIInteractionSystem::OnPointerPressed(PointerPressedEvent& pointerPressedEvent)
{
	this->elapsedTapTime = 0.0f;

	// Hit testing.
	this->currentButton = this->HitTest(pointerPressedEvent.position);
}

void UIInteractionSystem::OnPointerReleased(PointerReleasedEvent& pointerReleasedEvent)
{
	if (this->currentButton == nullptr)
	{
		return;
	}

	UI::Button* tappedButton = this->HitTest(pointerReleasedEvent.position);

	if (this->currentButton != tappedButton)
	{
		return;
	}

	// Notify listeners.
	if (this->elapsedTapTime < this->hoverThreshold)
	{
		auto entityTappedEvent = std::make_shared<EntityTappedEvent>(this->currentButton->entity);
		this->game->eventManager->QueueEvent(entityTappedEvent);

		// Play sound.
		auto playSoundAction = std::make_shared<PlaySoundAction>(L"Assets/sfx_button.wav");
		this->game->eventManager->QueueEvent(playSoundAction);
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

	return tappedButton;
}