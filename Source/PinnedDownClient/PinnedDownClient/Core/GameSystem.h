#pragma once

#include "../Helpers/StepTimer.h"
#include "Core\EventManager.h"
#include "Core\ResourceManager.h"

namespace PinnedDownClient
{
	namespace Core
	{
		class GameSystem
		{
		public:
			virtual void InitSystem(std::shared_ptr<Core::EventManager> eventManager, std::shared_ptr<Core::ResourceManager> resourceManager)
			{
				this->eventManager = eventManager;
				this->resourceManager = resourceManager;
			}

			virtual void Update(DX::StepTimer const& timer) {}
			virtual void Render() {}

		protected:
			std::shared_ptr<Core::EventManager> eventManager;
			std::shared_ptr<Core::ResourceManager> resourceManager;
		};
	}
}