#pragma once

#include "PinnedDownGame.h"
#include "Core\GameSystem.h"
#include "Core\IEventListener.h"

#include "Events\RenderTargetChangedEvent.h"

#include "Systems\UI\UIFactory.h"

using namespace PinnedDownClient::Core;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems::UI;

namespace PinnedDownClient
{
	namespace Systems
	{
		class UILoadingSystem : public Core::GameSystem, public Core::IEventListener
		{
		public:
			UILoadingSystem();

			void InitSystem(std::shared_ptr<GameInfrastructure> game);

		private:
			std::shared_ptr<UIFactory> uiFactory;

			void OnEvent(Event & event);

			void OnRenderTargetChanged(RenderTargetChangedEvent renderTargetChangedEvent);

			void LoadResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext);
			void UnloadResources();

			void LoadUI();
		};
	}
}