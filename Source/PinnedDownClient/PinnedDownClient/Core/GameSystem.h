#pragma once

#include "../Helpers/StepTimer.h"
#include "Core\EventManager.h"

namespace PinnedDownClient
{
	namespace Core
	{
		class GameSystem
		{
		public:
			virtual void InitSystem(std::shared_ptr<Core::EventManager> eventManager)
			{
				this->eventManager = eventManager;
			}

			virtual void Update(DX::StepTimer const& timer) {}
			virtual void Render() {}

		protected:
			std::shared_ptr<Core::EventManager> eventManager;
		};
	}
}