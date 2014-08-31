#pragma once

#include "IEventListener.h"
#include "GameSystem.h"

#include "Events\CardTappedEvent.h"

using namespace PinnedDownClient::Events;

namespace PinnedDownClient
{
	namespace Systems
	{
		class AssignmentSystem : public GameSystem, public IEventListener
		{
		public:
			AssignmentSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			Entity selectedCard;

			void OnEvent(Event & event);


		};
	}
}
