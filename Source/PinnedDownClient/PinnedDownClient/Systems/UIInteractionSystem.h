#pragma once

#include "PinnedDownGame.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Events\AudioEngineChangedEvent.h"
#include "EntityInitializedEvent.h"
#include "EntityRemovedEvent.h"
#include "Events\PointerReleasedEvent.h"

#include "Systems\UI\Button.h"

using namespace Microsoft::WRL;

using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems::UI;

namespace PinnedDownClient
{
	namespace Systems
	{
		class UIInteractionSystem : public GameSystem, public IEventListener
		{
		public:
			UIInteractionSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			ComPtr<IXAudio2> soundAudioEngine;

			std::list<Button> buttons;

			void LoadResources();

			void OnEvent(Event & event);

			void OnAudioEngineChanged(AudioEngineChangedEvent& audioEngineChangedEvent);
			void OnEntityInitialized(EntityInitializedEvent& entityInitializedEvent);
			void OnEntityRemoved(EntityRemovedEvent& entityRemovedEvent);
			void OnPointerReleased(PointerReleasedEvent& pointerReleasedEvent);
		};
	}
}