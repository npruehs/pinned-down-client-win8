#pragma once

#include "../Helpers/StepTimer.h"

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

			virtual void Update(DX::StepTimer const& timer) {}
			virtual void Render() {}

		protected:
			std::shared_ptr<GameInfrastructure> game;
		};
	}
}