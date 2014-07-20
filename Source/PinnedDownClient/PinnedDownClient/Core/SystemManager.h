#pragma once

#include <vector>
#include <memory>
#include "Core\GameSystem.h"
#include "Core\EventManager.h"
#include "Core\ResourceManager.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	class GameInfrastructure;

	namespace Core
	{
		class SystemManager
		{
		public:
			SystemManager(std::shared_ptr<GameInfrastructure> game);

			void AddSystem(GameSystem* system);
			void InitSystems();
			void Update(StepTimer const& timer);
			void Render();

		private:
			std::shared_ptr<GameInfrastructure> game;
			std::vector<std::shared_ptr<GameSystem>> systems;
		};
	}
}