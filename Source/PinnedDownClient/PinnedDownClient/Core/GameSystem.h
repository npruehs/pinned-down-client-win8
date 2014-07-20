#pragma once

#include "Util\StepTimer.h"

using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	class GameInfrastructure;

	namespace Core
	{
		class GameSystem
		{
		public:
			virtual void InitSystem(std::shared_ptr<GameInfrastructure> game)
			{
				this->game = game;
			}

			virtual void Update(StepTimer const& timer) {}
			virtual void Render() {}

		protected:
			std::shared_ptr<GameInfrastructure> game;
		};
	}
}