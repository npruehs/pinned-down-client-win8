#pragma once

#include "IEventListener.h"
#include "GameSystem.h"

#include "Events\PointerPressedEvent.h"
#include "Events\ScreenChangedEvent.h"

using namespace Windows::Networking;
using namespace Windows::Networking::Sockets;
using namespace Windows::Storage::Streams;

using namespace Microsoft::WRL;

using namespace PinnedDownClient::Events;

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
			bool connected = false;

			void OnEvent(Event & event);

			void OnScreenChanged(ScreenChangedEvent& screenChangedEvent);
			void OnPointerPressed(PointerPressedEvent& pointerPressedEvent);

			void InitSocket();
			void SendPacket();

			void SendPacketQueue();
			void RecvPacketQueue();
		};
	}
}
