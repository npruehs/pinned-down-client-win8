#pragma once

#include "PinnedDownGame.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Events\RenderTargetChangedEvent.h"

#include "Systems\Screens\Screen.h"
#include "Systems\UI\UIFactory.h"

using namespace Microsoft::WRL;

using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems::Screens;
using namespace PinnedDownClient::Systems::UI;

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

			void Update(float dt);

			void OnEvent(Event & event);

			void OnLoginSuccess();
			void OnRenderTargetChanged(RenderTargetChangedEvent renderTargetChangedEvent);
			
			void SetScreen(Screen* newScreen);
			void RefreshScreen();
		};
	}
}