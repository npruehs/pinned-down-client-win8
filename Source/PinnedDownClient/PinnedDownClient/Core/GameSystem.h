#pragma once

#include "../Helpers/StepTimer.h"
#include "Core\EventManager.h"
#include "Core\ResourceManager.h"

namespace PinnedDownClient
{
	class PinnedDownGame;

	namespace Core
	{
		class GameSystem
		{
		public:
			virtual void InitSystem(PinnedDownClient::PinnedDownGame* game)
			{
				this->game = game;
			}

			virtual void Update(DX::StepTimer const& timer) {}
			virtual void Render() {}

		protected:
			PinnedDownClient::PinnedDownGame* game;
		};
	}
}