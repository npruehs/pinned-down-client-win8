#include "pch.h"

#include "Game.h"
#include "Event.h"

#include "Resources\PinnedDownResourceManager.h"

#include "Systems\SoundEffectSystem.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Resources;
using namespace PinnedDownClient::Systems;


SoundEffectSystem::SoundEffectSystem()
{
}

void SoundEffectSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	// Register listener.
	this->game->eventManager->AddListener(this, AudioEngineChangedEvent::AudioEngineChangedEventType);
	this->game->eventManager->AddListener(this, EntityTappedEvent::EntityTappedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);

	this->LoadResources();
}

void SoundEffectSystem::LoadResources()
{
	auto resourceManager = static_cast<PinnedDownResourceManager*>(this->game->resourceManager.get());

	resourceManager->LoadAudioFromFile(
		this->soundAudioEngine.Get(),
		L"Assets/sfx_button.wav"
		);
}

void SoundEffectSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == AudioEngineChangedEvent::AudioEngineChangedEventType)
	{
		AudioEngineChangedEvent audioEngineChangedEvent = static_cast<AudioEngineChangedEvent&>(newEvent);
		this->OnAudioEngineChanged(audioEngineChangedEvent);
	}
	else if (newEvent.GetEventType() == EntityTappedEvent::EntityTappedEventType)
	{
		EntityTappedEvent& entityTappedEvent = static_cast<EntityTappedEvent&>(newEvent);
		this->OnEntityTapped(entityTappedEvent);
	}
	else if (newEvent.GetEventType() == TurnPhaseChangedEvent::TurnPhaseChangedEventType)
	{
		TurnPhaseChangedEvent& turnPhaseChangedEvent = static_cast<TurnPhaseChangedEvent&>(newEvent);
		this->OnTurnPhaseChanged(turnPhaseChangedEvent);
	}
}

void SoundEffectSystem::OnAudioEngineChanged(AudioEngineChangedEvent& audioEngineChangedEvent)
{
	this->soundAudioEngine = audioEngineChangedEvent.audioEngine;
}

void SoundEffectSystem::OnEntityTapped(EntityTappedEvent& entityTappedEvent)
{
	auto playSoundAction = std::make_shared<PlaySoundAction>(L"Assets/sfx_button.wav");
	this->game->eventManager->QueueEvent(playSoundAction);
}

void SoundEffectSystem::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
}