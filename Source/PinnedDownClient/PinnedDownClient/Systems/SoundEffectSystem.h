#pragma once

#include "IEventListener.h"
#include "GameSystem.h"

#include "Actions\PlaySoundAction.h"

#include "Events\AudioEngineChangedEvent.h"
#include "Events\CardPlayedEvent.h"
#include "Events\DefeatEvent.h"
#include "Events\EntityIdMappingCreatedEvent.h"
#include "Events\EntityTappedEvent.h"
#include "Events\FightResolvedEvent.h"
#include "Events\TurnPhaseChangedEvent.h"
#include "Events\VictoryEvent.h"

#include "Util\EntityIdMapping.h"

using namespace Microsoft::WRL;

using namespace PinnedDownClient::Events;
using namespace PinnedDownNet::Events;

namespace PinnedDownClient
{
	namespace Systems
	{
		class SoundEffectSystem : public GameSystem, public IEventListener
		{
		public:
			SoundEffectSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::shared_ptr<EntityIdMapping> entityIdMapping;
			ComPtr<IXAudio2> soundAudioEngine;

			void LoadResources();

			void OnEvent(Event & event);

			void OnAudioEngineChanged(AudioEngineChangedEvent& audioEngineChangedEvent);
			void OnCardPlayed(CardPlayedEvent& cardPlayedEvent);
			void OnDefeat(DefeatEvent& defeatEvent);
			void OnEntityIdMappingCreated(EntityIdMappingCreatedEvent& entityIdMappingCreatedEvent);
			void OnEntityTapped(EntityTappedEvent& entityTappedEvent);
			void OnFightResolved(FightResolvedEvent& fightResolvedEvent);
			void OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent);
			void OnVictory(VictoryEvent& victoryEvent);
		};
	}
}