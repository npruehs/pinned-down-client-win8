#pragma once

#include <vector>
#include <memory>
#include "Core\ISystem.h"
#include "Core\EventManager.h"

namespace PinnedDownClient
{
	namespace Core
	{
		class SystemManager
		{
		public:
			SystemManager(std::shared_ptr<EventManager> eventManager);

			void AddSystem(ISystem* system);
			void InitSystems();
			void Update(DX::StepTimer const& timer);

		private:
			std::shared_ptr<EventManager> eventManager;
			std::vector<std::shared_ptr<ISystem>> systems;
		};
	}
}