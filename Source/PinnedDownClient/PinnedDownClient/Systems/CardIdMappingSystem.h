#pragma once

#include "IEventListener.h"
#include "GameSystem.h"

#include "Data\CardFactory.h"

#include "Events\CardCreatedEvent.h"
#include "Events\EntityIdMappingCreatedEvent.h"

#include "Util\EntityIdMapping.h"


using namespace PinnedDownNet::Data;
using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Util;
using namespace PinnedDownClient::Events;


namespace PinnedDownClient
{
	namespace Systems
	{
		class CardIdMappingSystem : public GameSystem, public IEventListener
		{
		public:
			CardIdMappingSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::shared_ptr<CardFactory> cardFactory;
			std::shared_ptr<EntityIdMapping> entityIdMapping;

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(CardCreatedEvent);
			EVENT_HANDLER_DECLARATION(EntityIdMappingCreatedEvent);
		};
	}
}
