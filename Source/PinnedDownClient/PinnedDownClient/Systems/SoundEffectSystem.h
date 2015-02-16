#pragma once

#include "IEventListener.h"
#include "GameSystem.h"

#include "Actions\PlaySoundAction.h"

#include "Events\AudioEngineChangedEvent.h"
#include "Events\DefeatEvent.h"
#include "Events\EffectPlayedEvent.h"
#include "Events\EntityIdMappingCreatedEvent.h"
#include "Events\EntityTappedEvent.h"
#include "Events\FightResolvedEvent.h"
#include "Events\StarshipPlayedEvent.h"
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

			EVENT_HANDLER_DECLARATION(AudioEngineChangedEvent);
			EVENT_HANDLER_DECLARATION(DefeatEvent);
			EVENT_HANDLER_DECLARATION(EffectPlayedEvent);
			EVENT_HANDLER_DECLARATION(EntityIdMappingCreatedEvent);
			EVENT_HANDLER_DECLARATION(EntityTappedEvent);
			EVENT_HANDLER_DECLARATION(FightResolvedEvent);
			EVENT_HANDLER_DECLARATION(StarshipPlayedEvent);
			EVENT_HANDLER_DECLARATION(TurnPhaseChangedEvent);
			EVENT_HANDLER_DECLARATION(VictoryEvent);
		};
	}
}