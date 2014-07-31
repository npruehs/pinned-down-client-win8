#pragma once

#include "Game.h"
#include "Event.h"
#include "IEventListener.h"
#include "Logger.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Events
	{
		class EventLogger : public IEventListener
		{
		public:
			EventLogger(std::shared_ptr<PinnedDownCore::Game> game);

		private:
			std::shared_ptr<PinnedDownCore::Game> game;

			void OnEvent(Event & event);
		};
	}
}