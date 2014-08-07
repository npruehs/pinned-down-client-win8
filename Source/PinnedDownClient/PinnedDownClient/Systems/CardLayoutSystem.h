#pragma once

#include "IEventListener.h"
#include "GameSystem.h"

#include "Events\CardIdMappingCreatedEvent.h"
#include "Events\CardCopyCreatedEvent.h"
#include "Events\RenderTargetChangedEvent.h"

#include "Systems\UI\UIFactory.h"
#include "Systems\UI\Card.h"

#include "Util\CardIdMapping.h"


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
			std::shared_ptr<CardIdMapping> cardIdMapping;
			std::shared_ptr<UIFactory> uiFactory;
			ComPtr<ID2D1DeviceContext> d2dContext;
			std::list<std::shared_ptr<Card>> cards;

			void LoadResources();

			void OnEvent(Event & event);

			void OnCardIdMappingCreated(CardIdMappingCreatedEvent& cardIdMappingCreatedEvent);
			void OnCardCopyCreated(CardCopyCreatedEvent& cardCopyCreatedEvent);
			void OnRenderTargetChanged(RenderTargetChangedEvent& renderTargetChangedEvent);
		};
	}
}
