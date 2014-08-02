#pragma once

#include "PinnedDownGame.h"

#include "IEventListener.h"

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
				virtual void InitScreen(PinnedDownCore::Game* game)
				{
					this->game = game;
					this->uiFactory = std::make_shared<UIFactory>(game);
				}

				virtual void DeInitScreen() { }

				virtual void LoadResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext) { }
				virtual void UnloadResources() { }

				virtual void LoadUI() { }
				virtual void UnloadUI() { }

				virtual void Update(float dt) { }

				virtual ScreenName GetScreenName() = 0;

			protected:
				PinnedDownCore::Game* game;
				std::shared_ptr<UIFactory> uiFactory;
			};
		}
	}
}