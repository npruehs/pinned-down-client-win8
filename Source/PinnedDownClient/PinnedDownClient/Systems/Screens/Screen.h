#pragma once

#include "PinnedDownGame.h"

#include "IEventListener.h"

#include "Systems\UI\UIFactory.h"
#include "Systems\Screens\ScreenName.h"

#include "Util\ClientIdMapping.h"
#include "Util\EntityIdMapping.h"

using namespace PinnedDownClient::Systems::UI;
using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace Screens
		{
			class Screen
			{
			public:
				virtual void InitScreen(PinnedDownCore::Game* game, std::shared_ptr<ClientIdMapping> clientIdMapping, std::shared_ptr<EntityIdMapping> entityIdMapping)
				{
					this->game = game;
					this->clientIdMapping = clientIdMapping;
					this->entityIdMapping = entityIdMapping;

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
				std::shared_ptr<EntityIdMapping> entityIdMapping;
				std::shared_ptr<ClientIdMapping> clientIdMapping;
			};
		}
	}
}