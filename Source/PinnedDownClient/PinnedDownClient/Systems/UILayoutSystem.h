#pragma once

#include "PinnedDownGame.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Events\AppWindowChangedEvent.h"
#include "Events\AppWindowSizeChangedEvent.h"
#include "EntityInitializedEvent.h"
#include "EntityRemovedEvent.h"

#include "Systems\UI\Anchor.h"
#include "Systems\UI\Panel.h"
#include "Systems\UI\UIFactory.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems::UI;

namespace PinnedDownClient
{
	namespace Systems
	{
		class UILayoutSystem : public GameSystem, public IEventListener
		{
		public:
			UILayoutSystem();

			void InitSystem(PinnedDownCore::Game* game);
			void Update(float dt);

		private:
			void OnEvent(Event & event);

			std::shared_ptr<UIFactory> uiFactory;
			Panel rootPanel;
			std::list<Anchor> anchors;

			void CreateRootPanel();
			void ResizeRootPanel(Vector2F newSize);

			void OnAppWindowChanged(AppWindowChangedEvent appWindowChangedEvent);
			void OnAppWindowSizeChanged(AppWindowSizeChangedEvent appWindowSizeChangedEvent);
			void OnEntityInitialized(EntityInitializedEvent entityInitializedEvent);
			void OnEntityRemoved(EntityRemovedEvent entityRemovedEvent);
		};
	}
}