#pragma once

#include "IEventListener.h"
#include "GameSystem.h"

#include "Events\DisconnectedFromServerEvent.h"
#include "Events\MatchEndedEvent.h"

#include "Util\EntityIdMapping.h"


using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Util;


namespace PinnedDownClient
{
	namespace Systems
	{
		class EntityIdMappingSystem : public GameSystem, public IEventListener
		{
		public:
			EntityIdMappingSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::shared_ptr<EntityIdMapping> entityIdMapping;

			EVENT_HANDLER_DECLARATION(DisconnectedFromServerEvent);
			EVENT_HANDLER_DECLARATION(MatchEndedEvent);

			void OnEvent(Event & event);
		};
	}
}
