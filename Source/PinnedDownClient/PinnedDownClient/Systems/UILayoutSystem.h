#pragma once

#include "PinnedDownGame.h"
#include "Core\GameSystem.h"
#include "Core\IEventListener.h"

#include "Events\AppWindowChangedEvent.h"
#include "Events\AppWindowSizeChangedEvent.h"

using namespace PinnedDownClient::Core;
using namespace PinnedDownClient::Events;

namespace PinnedDownClient
{
	namespace Systems
	{
		class UILayoutSystem : public Core::GameSystem, public Core::IEventListener
		{
		public:
			UILayoutSystem();

			void InitSystem(std::shared_ptr<GameInfrastructure> game);

		private:
			void OnEvent(Event & event);

			int rootPanelEntity;

			void CreateRootPanel();
			void ResizeRootPanel(Vector2F newSize);

			void OnAppWindowChanged(AppWindowChangedEvent appWindowChangedEvent);
			void OnAppWindowSizeChanged(AppWindowSizeChangedEvent appWindowSizeChangedEvent);
		};
	}
}