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

			EVENT_HANDLER_DECLARATION(EntityInitializedEvent);
			EVENT_HANDLER_DECLARATION(LocalizedTextChangedEvent);
			EVENT_HANDLER_DECLARATION(ResourceLoadedEvent);

			void LocalizeText(Entity entity);
		};
	}
}