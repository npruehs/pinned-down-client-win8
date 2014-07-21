#pragma once

#include "PinnedDownGame.h"
#include "Core\GameSystem.h"
#include "Core\IEventListener.h"

using namespace PinnedDownClient::Core;

namespace PinnedDownClient
{
	namespace Systems
	{
		class UILoadingSystem : public Core::GameSystem, public Core::IEventListener
		{
		public:
			UILoadingSystem();

			void InitSystem(std::shared_ptr<GameInfrastructure> game);

		private:
			void OnEvent(Event & event);

			void LoadUI();
		};
	}
}