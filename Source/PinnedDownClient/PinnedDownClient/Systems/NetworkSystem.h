#pragma once

#include "GameInfrastructure.h"

#include "Core\IEventListener.h"
#include "Core\GameSystem.h"

#include "Events\PointerPressedEvent.h"
#include "Events\ScreenChangedEvent.h"

using namespace Windows::Networking;
using namespace Windows::Networking::Sockets;
using namespace Windows::Storage::Streams;

using namespace Microsoft::WRL;

using namespace PinnedDownClient::Core;
using namespace PinnedDownClient::Events;

namespace PinnedDownClient
{
	namespace Systems
	{
		class NetworkSystem : public Core::GameSystem, public Core::IEventListener
		{
		public:
			NetworkSystem();

			void InitSystem(std::shared_ptr<GameInfrastructure> game);

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
		};
	}
}
