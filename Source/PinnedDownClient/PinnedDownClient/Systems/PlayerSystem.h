#pragma once

#include "IEventListener.h"
#include "GameSystem.h"

#include "Data\PlayerFactory.h"

#include "Events\ClientIdMappingCreatedEvent.h"
#include "Events\PlayerAddedEvent.h"

#include "Util\ClientIdMapping.h"
#include "Util\EntityIdMapping.h"

using namespace PinnedDownClient::Events;
using namespace PinnedDownNet::Events;
using namespace PinnedDownNet::Data;


namespace PinnedDownClient
{
	namespace Systems
	{
		class PlayerSystem : public GameSystem, public IEventListener
		{
		public:
			PlayerSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::shared_ptr<ClientIdMapping> clientIdMapping;
			std::shared_ptr<PlayerFactory> playerFactory;

			void OnEvent(Event & event);

			void OnClientIdMappingCreated(ClientIdMappingCreatedEvent& clientIdMappingCreatedEvent);
			void OnPlayerAdded(PlayerAddedEvent& playerAddedEvent);
		};
	}
}
