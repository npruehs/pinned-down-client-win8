#pragma once

#include "EntityManager.h"
#include "IEventListener.h"
#include "GameSystem.h"


#include "Events\LoginSuccessEvent.h"

#include "Util\ClientIdMapping.h"


using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Util;


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

			void OnLoginSuccess(LoginSuccessEvent& loginSuccessEvent);
		};
	}
}
