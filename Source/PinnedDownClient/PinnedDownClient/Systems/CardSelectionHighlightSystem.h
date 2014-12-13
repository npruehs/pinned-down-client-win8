#pragma once

#include "IEventListener.h"
#include "GameSystem.h"

#include "Events\CardSelectedEvent.h"
#include "Events\CardDeselectedEvent.h"
#include "Events\RenderTargetChangedEvent.h"

#include "Systems\UI\UIFactory.h"


using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems::UI;
using namespace PinnedDownClient::Util;


namespace PinnedDownClient
{
	namespace Systems
	{
		class CardSelectionHighlightSystem : public GameSystem, public IEventListener
		{
		public:
			CardSelectionHighlightSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::shared_ptr<UIFactory> uiFactory;
			ComPtr<ID2D1DeviceContext> d2dContext;

			Entity currentHighlight;

			void LoadResources();

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(CardSelectedEvent);
			EVENT_HANDLER_DECLARATION(CardDeselectedEvent);
			EVENT_HANDLER_DECLARATION(RenderTargetChangedEvent);
		};
	}
}
