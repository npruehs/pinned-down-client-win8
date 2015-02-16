#pragma once

#include "EntityManager.h"
#include "IEventListener.h"
#include "GameSystem.h"

#include "Events\DisconnectedFromServerEvent.h"
#include "Events\LoginSuccessEvent.h"

#include "Util\ClientIdMapping.h"


using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Util;
using namespace PinnedDownNet::Events;


namespace PinnedDownClient
{
	namespace Systems
	{
		class ClientIdMappingSystem : public GameSystem, public IEventListener
		{
		public:
			ClientIdMappingSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::shared_ptr<ClientIdMapping> clientIdMapping;

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(DisconnectedFromServerEvent);
			EVENT_HANDLER_DECLARATION(LoginSuccessEvent);
		};
	}
}
