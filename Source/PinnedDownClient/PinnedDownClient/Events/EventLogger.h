#pragma once

#include "GameInfrastructure.h"
#include "Core\Event.h"
#include "Core\IEventListener.h"
#include "Util\Logger.h"

using namespace PinnedDownClient;
using namespace PinnedDownClient::Core;
using namespace PinnedDownClient::Util;

namespace PinnedDownClient
{
	namespace Events
	{
		class EventLogger : public Core::IEventListener
		{
		public:
			EventLogger(std::shared_ptr<GameInfrastructure> game);

		private:
			std::shared_ptr<GameInfrastructure> game;

			void OnEvent(Event & event);
		};
	}
}