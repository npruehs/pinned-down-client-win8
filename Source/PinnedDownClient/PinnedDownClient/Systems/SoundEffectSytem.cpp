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
	this->game->eventManager->AddListener(this, CardPlayedEvent::CardPlayedEventType);
	this->game->eventManager->AddListener(this, DefeatEvent::DefeatEventType);
	this->game->eventManager->AddListener(this, EntityIdMappingCreatedEvent::EntityIdMappingCreatedEventType);
	this->game->eventManager->AddListener(this, EntityTappedEvent::EntityTappedEventType);
	this->game->eventManager->AddListener(this, FightResolvedEvent::FightResolvedEventType);
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
	if (newEvent.GetEventType() == AudioEngineChangedEvent::AudioEngineChangedEventType)
	{
		AudioEngineChangedEvent& audioEngineChangedEvent = static_cast<AudioEngineChangedEvent&>(newEvent);
		this->OnAudioEngineChanged(audioEngineChangedEvent);
	}
	else if(newEvent.GetEventType() == CardPlayedEvent::CardPlayedEventType)
	{
		CardPlayedEvent& cardPlayedEvent = static_cast<CardPlayedEvent&>(newEvent);
		this->OnCardPlayed(cardPlayedEvent);
	}
	else if (newEvent.GetEventType() == DefeatEvent::DefeatEventType)
	{
		DefeatEvent& defeatEvent = static_cast<DefeatEvent&>(newEvent);
		this->OnDefeat(defeatEvent);
	}
	else if (newEvent.GetEventType() == EntityIdMappingCreatedEvent::EntityIdMappingCreatedEventType)
	{
		EntityIdMappingCreatedEvent& entityIdMappingCreatedEvent = static_cast<EntityIdMappingCreatedEvent&>(newEvent);
		this->OnEntityIdMappingCreated(entityIdMappingCreatedEvent);
	}
	else if (newEvent.GetEventType() == EntityTappedEvent::EntityTappedEventType)
	{
		EntityTappedEvent& entityTappedEvent = static_cast<EntityTappedEvent&>(newEvent);
		this->OnEntityTapped(entityTappedEvent);
	}
	else if (newEvent.GetEventType() == FightResolvedEvent::FightResolvedEventType)
	{
		FightResolvedEvent& fightResolvedEvent = static_cast<FightResolvedEvent&>(newEvent);
		this->OnFightResolved(fightResolvedEvent);
	}
	else if (newEvent.GetEventType() == TurnPhaseChangedEvent::TurnPhaseChangedEventType)
	{
		TurnPhaseChangedEvent& turnPhaseChangedEvent = static_cast<TurnPhaseChangedEvent&>(newEvent);
		this->OnTurnPhaseChanged(turnPhaseChangedEvent);
	}
	else if (newEvent.GetEventType() == VictoryEvent::VictoryEventType)
	{
		VictoryEvent& victoryEvent = static_cast<VictoryEvent&>(newEvent);
		this->OnVictory(victoryEvent);
	}
}

void SoundEffectSystem::OnAudioEngineChanged(AudioEngineChangedEvent& audioEngineChangedEvent)
{
	this->soundAudioEngine = audioEngineChangedEvent.audioEngine;
}

void SoundEffectSystem::OnCardPlayed(CardPlayedEvent& cardPlayedEvent)
{
	auto entityId = this->entityIdMapping->ServerToClientId(cardPlayedEvent.serverEntity);
	auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(entityId, CardComponent::CardComponentType);

	if (cardComponent->cardType == CardType::Effect)
	{
		auto playSoundAction = std::make_shared<PlaySoundAction>(L"Assets/Sounds/EffectPlayed.wav");
		this->game->eventManager->QueueEvent(playSoundAction);
	}
	else if (cardComponent->cardType == CardType::Starship)
	{
		auto playSoundAction = std::make_shared<PlaySoundAction>(L"Assets/Sounds/StarshipPlayed.wav");
		this->game->eventManager->QueueEvent(playSoundAction);
	}
}

void SoundEffectSystem::OnDefeat(DefeatEvent& defeatEvent)
{
	auto playSoundAction = std::make_shared<PlaySoundAction>(L"Assets/Sounds/GameOver.wav");
	this->game->eventManager->QueueEvent(playSoundAction);
}

void SoundEffectSystem::OnEntityIdMappingCreated(EntityIdMappingCreatedEvent& entityIdMappingCreatedEvent)
{
	this->entityIdMapping = entityIdMappingCreatedEvent.entityIdMapping;
}

void SoundEffectSystem::OnEntityTapped(EntityTappedEvent& entityTappedEvent)
{
	auto playSoundAction = std::make_shared<PlaySoundAction>(L"Assets/Sounds/EntityTapped.wav");
	this->game->eventManager->QueueEvent(playSoundAction);
}

void SoundEffectSystem::OnFightResolved(FightResolvedEvent& fightResolvedEvent)
{
	auto playSoundAction = std::make_shared<PlaySoundAction>(L"Assets/Sounds/FightResolved.wav");
	this->game->eventManager->QueueEvent(playSoundAction);
}

void SoundEffectSystem::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	if (turnPhaseChangedEvent.newTurnPhase == TurnPhase::Attack)
	{
		auto playSoundAction = std::make_shared<PlaySoundAction>(L"Assets/Sounds/AttackPhase.wav");
		this->game->eventManager->QueueEvent(playSoundAction);
	}
	else if (turnPhaseChangedEvent.newTurnPhase == TurnPhase::Jump)
	{
		auto playSoundAction = std::make_shared<PlaySoundAction>(L"Assets/Sounds/JumpPhase.wav");
		this->game->eventManager->QueueEvent(playSoundAction);
	}
}

void SoundEffectSystem::OnVictory(VictoryEvent& victoryEvent)
{
	auto playSoundAction = std::make_shared<PlaySoundAction>(L"Assets/Sounds/GameOver.wav");
	this->game->eventManager->QueueEvent(playSoundAction);
}