#pragma once

#include <memory>

#include "Game.h"
#include "PinnedDownNet.h"
#include "Systems\Network\ClientActionWriter.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Systems
	{
		namespace Network
		{
			class ClientActionDispatcher : public IEventListener
			{
			public:
				ClientActionDispatcher(Game* game, std::shared_ptr<ClientActionWriter> clientActionWriter);
				~ClientActionDispatcher();

			private:
				Game* game;
				std::shared_ptr<ClientActionWriter> clientActionWriter;

				void OnEvent(Event & event);
			};
		}
	}
}