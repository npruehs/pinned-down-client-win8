#pragma once

#include "PinnedDownGame.h"
#include "Core\GameSystem.h"
#include "Core\IEventListener.h"

#include "Events\RenderTargetChangedEvent.h"

#include "Systems\Screens\Screen.h"
#include "Systems\UI\UIFactory.h"

using namespace Microsoft::WRL;

using namespace PinnedDownClient::Core;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems::Screens;
using namespace PinnedDownClient::Systems::UI;

namespace PinnedDownClient
{
	namespace Systems
	{
		class ScreenSystem : public Core::GameSystem, public Core::IEventListener
		{
		public:
			ScreenSystem();

			void InitSystem(std::shared_ptr<GameInfrastructure> game);

		private:
			std::shared_ptr<UIFactory> uiFactory;
			std::shared_ptr<Screen> currentScreen;
			ComPtr<ID2D1DeviceContext> d2dContext;

			void Update(StepTimer const& timer);

			void OnEvent(Event & event);

			void OnLoginSuccess();
			void OnRenderTargetChanged(RenderTargetChangedEvent renderTargetChangedEvent);
			
			void SetScreen(Screen* newScreen);
			void RefreshScreen();
		};
	}
}