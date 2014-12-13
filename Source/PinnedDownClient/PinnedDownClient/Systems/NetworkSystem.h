#pragma once

#include "IEventListener.h"
#include "GameSystem.h"

#include "Actions\ConnectToServerAction.h"

#include "Services\MobileServices\PinnedDownMobileService.h"

#include "Systems\Network\ServerEventReader.h"
#include "Systems\Network\ClientActionWriter.h"
#include "Systems\Network\ClientActionDispatcher.h"

using namespace Windows::Networking;
using namespace Windows::Networking::Sockets;
using namespace Windows::Storage::Streams;

using namespace Microsoft::WRL;

using namespace PinnedDownClient::Events;

using namespace PinnedDownClient::Systems::Network;

namespace PinnedDownClient
{
	namespace Systems
	{
		class NetworkSystem : public GameSystem, public IEventListener
		{
		public:
			NetworkSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			StreamSocket^ clientSocket;
			HostName^ serverHost;
			DataWriter^ dataWriter;
			DataReader^ dataReader;
			std::shared_ptr<ServerEventReader> serverEventReader;
			std::shared_ptr<ClientActionWriter> clientActionWriter;
			std::shared_ptr<ClientActionDispatcher> clientActionDispatcher;
			azure::mobile::user user;

			bool connected = false;

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(ConnectToServerAction);

			void InitSocket();

			void RecvPacketLoop();
		};
	}
}
