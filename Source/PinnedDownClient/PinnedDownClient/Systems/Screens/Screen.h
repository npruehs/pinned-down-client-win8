#pragma once

#include "PinnedDownGame.h"

#include "Core\IEventListener.h"

#include "Systems\UI\UIFactory.h"
#include "Systems\Screens\ScreenName.h"

using namespace PinnedDownClient::Systems::UI;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace Screens
		{
			class Screen
			{
			public:
				void InitScreen(std::shared_ptr<GameInfrastructure> game)
				{
					this->game = game;
					this->uiFactory = std::make_shared<UIFactory>(game);
				}

				virtual void LoadResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext) { }
				virtual void UnloadResources() { }

				virtual void LoadUI() { }
				virtual void UnloadUI() { }

				virtual ScreenName GetScreenName() = 0;

			protected:
				std::shared_ptr<GameInfrastructure> game;
				std::shared_ptr<UIFactory> uiFactory;
			};
		}
	}
}