#pragma once

#include "PinnedDownGame.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "EntityInitializedEvent.h"
#include "Events\LocalizedTextChangedEvent.h"
#include "Events\ResourceLoadedEvent.h"

#include "Util\LuaInstance.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Util;


namespace PinnedDownClient
{
	namespace Systems
	{
		class LocalizationSystem : public GameSystem, public IEventListener
		{
		public:
			LocalizationSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::shared_ptr<LuaInstance> lua;

			void LoadLocalizationData();

			void OnEvent(Event & event);

			void OnEntityInitialized(EntityInitializedEvent& entityInitializedEvent);
			void OnLocalizedTextChanged(LocalizedTextChangedEvent& localizedTextChangedEvent);
			void OnResourceLoaded(ResourceLoadedEvent& resourceLoadedEvent);

			void LocalizeText(Entity entity);
		};
	}
}