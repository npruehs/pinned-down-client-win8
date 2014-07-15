#pragma once

#include <vector>
#include <memory>
#include "Core\GameSystem.h"
#include "Core\EventManager.h"
#include "Core\ResourceManager.h"

namespace PinnedDownClient
{
	class PinnedDownGame;

	namespace Core
	{
		class SystemManager
		{
		public:
			SystemManager(PinnedDownClient::PinnedDownGame* game);

			void AddSystem(GameSystem* system);
			void InitSystems();
			void Update(DX::StepTimer const& timer);
			void Render();

		private:
			PinnedDownClient::PinnedDownGame* game;
			std::vector<std::shared_ptr<GameSystem>> systems;
		};
	}
}