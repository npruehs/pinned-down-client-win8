#pragma once

#include "PinnedDownGame.h"
#include "Core\GameSystem.h"
#include "Core\IEventListener.h"

#include "Events\AppWindowChangedEvent.h"
#include "Events\AppWindowSizeChangedEvent.h"
#include "Events\EntityInitializedEvent.h"
#include "Events\EntityRemovedEvent.h"

#include "Systems\UI\Anchor.h"
#include "Systems\UI\Panel.h"
#include "Systems\UI\UIFactory.h"

using namespace PinnedDownClient::Core;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems::UI;

namespace PinnedDownClient
{
	namespace Systems
	{
		class UILayoutSystem : public Core::GameSystem, public Core::IEventListener
		{
		public:
			UILayoutSystem();

			void InitSystem(std::shared_ptr<GameInfrastructure> game);
			void Update(StepTimer const& timer);

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