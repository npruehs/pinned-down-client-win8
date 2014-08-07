#pragma once

#include "IEventListener.h"
#include "GameSystem.h"

#include "Data\CardFactory.h"

#include "Events\CardCreatedEvent.h"

#include "Util\CardIdMapping.h"


using namespace PinnedDownNet::Data;
using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Util;


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
			std::shared_ptr<CardIdMapping> cardIdMapping;

			void OnEvent(Event & event);

			void OnCardCreated(CardCreatedEvent& cardCreatedEvent);
		};
	}
}
