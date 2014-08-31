#pragma once

#include "IEventListener.h"
#include "GameSystem.h"

#include "Events\EntityIdMappingCreatedEvent.h"
#include "Events\CardCreatedEvent.h"
#include "Events\CardRemovedEvent.h"
#include "Events\RenderTargetChangedEvent.h"

#include "Systems\UI\UIFactory.h"
#include "Systems\UI\Card.h"

#include "Util\EntityIdMapping.h"


using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems::UI;
using namespace PinnedDownClient::Util;


namespace PinnedDownClient
{
	namespace Systems
	{
		class CardLayoutSystem : public GameSystem, public IEventListener
		{
		public:
			CardLayoutSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::shared_ptr<EntityIdMapping> entityIdMapping;
			std::shared_ptr<UIFactory> uiFactory;
			ComPtr<ID2D1DeviceContext> d2dContext;
			std::list<std::shared_ptr<Card>> cards;

			void LoadResources();

			void OnEvent(Event & event);

			void OnCardCreated(CardCreatedEvent& cardCreatedEvent);
			void OnCardRemoved(CardRemovedEvent& cardRemovedEvent);
			void OnEntityIdMappingCreated(EntityIdMappingCreatedEvent& entityIdMappingCreatedEvent);
			void OnRenderTargetChanged(RenderTargetChangedEvent& renderTargetChangedEvent);
		};
	}
}
