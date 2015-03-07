#pragma once

#include "PinnedDownGame.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Events\ClientIdMappingCreatedEvent.h"
#include "Events\ClientVersionVerifiedEvent.h"
#include "Events\DisconnectedFromServerEvent.h"
#include "Events\EntityIdMappingCreatedEvent.h"
#include "Events\GameStartedEvent.h"
#include "Events\LocalizationDataLoadedEvent.h"
#include "Events\MatchEndedEvent.h"
#include "Events\RenderTargetChangedEvent.h"

#include "Systems\Screens\Screen.h"
#include "Systems\UI\UIFactory.h"

#include "Util\ClientIdMapping.h"
#include "Util\EntityIdMapping.h"

using namespace Microsoft::WRL;

using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems::Screens;
using namespace PinnedDownClient::Systems::UI;
using namespace PinnedDownClient::Util;
using namespace PinnedDownNet::Events;

namespace PinnedDownClient
{
	namespace Systems
	{
		class ScreenSystem : public GameSystem, public IEventListener
		{
		public:
			ScreenSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::shared_ptr<UIFactory> uiFactory;
			std::shared_ptr<Screen> currentScreen;
			ComPtr<ID2D1DeviceContext> d2dContext;
			std::shared_ptr<EntityIdMapping> entityIdMapping;
			std::shared_ptr<ClientIdMapping> clientIdMapping;

			void Update(float dt);

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(ClientIdMappingCreatedEvent);
			EVENT_HANDLER_DECLARATION(ClientVersionVerifiedEvent);
			EVENT_HANDLER_DECLARATION(DisconnectedFromServerEvent);
			EVENT_HANDLER_DECLARATION(EntityIdMappingCreatedEvent);
			EVENT_HANDLER_DECLARATION(GameStartedEvent);
			EVENT_HANDLER_DECLARATION(LocalizationDataLoadedEvent);
			EVENT_HANDLER_DECLARATION(MatchEndedEvent);
			EVENT_HANDLER_DECLARATION(RenderTargetChangedEvent);

			void SetScreen(std::shared_ptr<Screen> newScreen);
			void RefreshScreen();
		};
	}
}