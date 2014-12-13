#pragma once

#include "IEventListener.h"
#include "GameSystem.h"

#include "Actions\PlaySoundAction.h"

#include "Events\AudioEngineChangedEvent.h"
#include "Events\EntityTappedEvent.h"
#include "Events\TurnPhaseChangedEvent.h"

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
			ComPtr<IXAudio2> soundAudioEngine;

			void LoadResources();

			void OnEvent(Event & event);

			void OnAudioEngineChanged(AudioEngineChangedEvent& audioEngineChangedEvent);
			void OnEntityTapped(EntityTappedEvent& entityTappedEvent);
			void OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent);
		};
	}
}
