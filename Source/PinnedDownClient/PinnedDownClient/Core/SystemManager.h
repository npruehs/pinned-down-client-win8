#pragma once

#include "Core\ISystem.h"
#include <vector>
#include <memory>

namespace PinnedDownClient
{
	namespace Core
	{
		class SystemManager
		{
		public:
			SystemManager();

			void AddSystem(ISystem* system);
			void InitSystems();
			void Update(DX::StepTimer const& timer);

		private:
			std::vector<std::shared_ptr<ISystem>> systems;
		};
	}
}