#include "pch.h"
#include "Event.h"
#include "Systems\UIInteractionSystem.h"

#include "Actions\PlaySoundAction.h"

#include "Components\BoundsComponent.h"
#include "Components\ScreenPositionComponent.h"
#include "Components\TappableComponent.h"

#include "EntityRemovedEvent.h"
#include "Events\EntityTappedEvent.h"

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
	this->game->eventManager->AddListener(this, PointerReleasedEvent::PointerReleasedEventType);

	this->LoadResources();
}

void UIInteractionSystem::LoadResources()
{
	auto resourceManager = static_cast<PinnedDownResourceManager*>(this->game->resourceManager.get());

	resourceManager->LoadAudioFromFile(
		this->soundAudioEngine.Get(),
		L"Assets/chord.wav"
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
	else if (newEvent.GetEventType() == PointerReleasedEvent::PointerReleasedEventType)
	{
		PointerReleasedEvent pointerReleasedEvent = static_cast<PointerReleasedEvent&>(newEvent);
		this->OnPointerReleased(pointerReleasedEvent);
	}
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

void UIInteractionSystem::OnPointerReleased(PointerReleasedEvent& pointerReleasedEvent)
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
		auto entityTappedEvent = std::make_shared<EntityTappedEvent>(tappedButton->entity);
		this->game->eventManager->QueueEvent(entityTappedEvent);

		// Play sound.
		auto playSoundAction = std::make_shared<PlaySoundAction>(L"Assets/chord.wav");
		this->game->eventManager->QueueEvent(playSoundAction);
	}
}