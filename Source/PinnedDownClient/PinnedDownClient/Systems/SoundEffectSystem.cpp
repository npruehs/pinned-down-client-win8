#include "pch.h"

#include "Game.h"
#include "Event.h"

#include "Components\CardComponent.h"

#include "Resources\PinnedDownResourceManager.h"

#include "Systems\SoundEffectSystem.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Components;
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
	this->game->eventManager->AddListener(this, DefeatEvent::DefeatEventType);
	this->game->eventManager->AddListener(this, EffectPlayedEvent::EffectPlayedEventType);
	this->game->eventManager->AddListener(this, EntityIdMappingCreatedEvent::EntityIdMappingCreatedEventType);
	this->game->eventManager->AddListener(this, EntityTappedEvent::EntityTappedEventType);
	this->game->eventManager->AddListener(this, FightResolvedEvent::FightResolvedEventType);
	this->game->eventManager->AddListener(this, StarshipPlayedEvent::StarshipPlayedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
	this->game->eventManager->AddListener(this, VictoryEvent::VictoryEventType);

	this->LoadResources();
}

void SoundEffectSystem::LoadResources()
{
	auto resourceManager = static_cast<PinnedDownResourceManager*>(this->game->resourceManager.get());

	resourceManager->LoadAudioFromFile(
		this->soundAudioEngine.Get(),
		L"Assets/Sounds/AttackPhase.wav"
		);
	resourceManager->LoadAudioFromFile(
		this->soundAudioEngine.Get(),
		L"Assets/Sounds/EffectPlayed.wav"
		);
	resourceManager->LoadAudioFromFile(
		this->soundAudioEngine.Get(),
		L"Assets/Sounds/EntityTapped.wav"
		);
	resourceManager->LoadAudioFromFile(
		this->soundAudioEngine.Get(),
		L"Assets/Sounds/FightResolved.wav"
		);
	resourceManager->LoadAudioFromFile(
		this->soundAudioEngine.Get(),
		L"Assets/Sounds/GameOver.wav"
		);
	resourceManager->LoadAudioFromFile(
		this->soundAudioEngine.Get(),
		L"Assets/Sounds/JumpPhase.wav"
		);
	resourceManager->LoadAudioFromFile(
		this->soundAudioEngine.Get(),
		L"Assets/Sounds/StarshipPlayed.wav"
		);
}

void SoundEffectSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(AudioEngineChangedEvent);
	CALL_EVENT_HANDLER(DefeatEvent);
	CALL_EVENT_HANDLER(EffectPlayedEvent);
	CALL_EVENT_HANDLER(EntityIdMappingCreatedEvent);
	CALL_EVENT_HANDLER(EntityTappedEvent);
	CALL_EVENT_HANDLER(FightResolvedEvent);
	CALL_EVENT_HANDLER(StarshipPlayedEvent);
	CALL_EVENT_HANDLER(TurnPhaseChangedEvent);
	CALL_EVENT_HANDLER(VictoryEvent);
}

EVENT_HANDLER_DEFINITION(SoundEffectSystem, AudioEngineChangedEvent)
{
	this->soundAudioEngine = data.audioEngine;
}

EVENT_HANDLER_DEFINITION(SoundEffectSystem, DefeatEvent)
{
	auto playSoundAction = std::make_shared<PlaySoundAction>(L"Assets/Sounds/GameOver.wav");
	this->game->eventManager->QueueEvent(playSoundAction);
}

EVENT_HANDLER_DEFINITION(SoundEffectSystem, EffectPlayedEvent)
{
	auto playSoundAction = std::make_shared<PlaySoundAction>(L"Assets/Sounds/EffectPlayed.wav");
	this->game->eventManager->QueueEvent(playSoundAction);
}

EVENT_HANDLER_DEFINITION(SoundEffectSystem, EntityIdMappingCreatedEvent)
{
	this->entityIdMapping = data.entityIdMapping;
}

EVENT_HANDLER_DEFINITION(SoundEffectSystem, EntityTappedEvent)
{
	auto playSoundAction = std::make_shared<PlaySoundAction>(L"Assets/Sounds/EntityTapped.wav");
	this->game->eventManager->QueueEvent(playSoundAction);
}

EVENT_HANDLER_DEFINITION(SoundEffectSystem, FightResolvedEvent)
{
	auto playSoundAction = std::make_shared<PlaySoundAction>(L"Assets/Sounds/FightResolved.wav");
	this->game->eventManager->QueueEvent(playSoundAction);
}

EVENT_HANDLER_DEFINITION(SoundEffectSystem, StarshipPlayedEvent)
{
	auto playSoundAction = std::make_shared<PlaySoundAction>(L"Assets/Sounds/StarshipPlayed.wav");
	this->game->eventManager->QueueEvent(playSoundAction);
}

EVENT_HANDLER_DEFINITION(SoundEffectSystem, TurnPhaseChangedEvent)
{
	if (data.newTurnPhase == TurnPhase::Attack)
	{
		auto playSoundAction = std::make_shared<PlaySoundAction>(L"Assets/Sounds/AttackPhase.wav");
		this->game->eventManager->QueueEvent(playSoundAction);
	}
	else if (data.newTurnPhase == TurnPhase::Jump)
	{
		auto playSoundAction = std::make_shared<PlaySoundAction>(L"Assets/Sounds/JumpPhase.wav");
		this->game->eventManager->QueueEvent(playSoundAction);
	}
}

EVENT_HANDLER_DEFINITION(SoundEffectSystem, VictoryEvent)
{
	auto playSoundAction = std::make_shared<PlaySoundAction>(L"Assets/Sounds/GameOver.wav");
	this->game->eventManager->QueueEvent(playSoundAction);
}