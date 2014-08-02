#pragma once

#include "IEventListener.h"
#include "GameSystem.h"

#include "Actions\ConnectToServerAction.h"
#include "Events\PointerPressedEvent.h"

#include "Services\MobileServices\PinnedDownMobileService.h"

#include "Systems\Network\ServerEventReader.h"
#include "Systems\Network\ClientActionWriter.h"


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
			azure::mobile::user user;

			bool connected = false;

			void OnEvent(Event & event);

			void OnConnectToServer();
			void OnPointerPressed(PointerPressedEvent& pointerPressedEvent);

			void InitSocket();
			void SendPacket();

			void RecvPacketLoop();
		};
	}
}
