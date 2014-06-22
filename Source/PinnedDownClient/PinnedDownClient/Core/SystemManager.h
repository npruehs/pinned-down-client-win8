#pragma once

#include <vector>
#include <memory>
#include "Core\GameSystem.h"
#include "Core\EventManager.h"

namespace PinnedDownClient
{
	namespace Core
	{
		class SystemManager
		{
		public:
			SystemManager(std::shared_ptr<EventManager> eventManager);

			void AddSystem(GameSystem* system);
			void InitSystems();
			void Update(DX::StepTimer const& timer);
			void Render();

		private:
			std::shared_ptr<EventManager> eventManager;
			std::vector<std::shared_ptr<GameSystem>> systems;
		};
	}
}