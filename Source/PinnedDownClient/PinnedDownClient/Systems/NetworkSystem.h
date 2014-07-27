#pragma once

#include "GameInfrastructure.h"
#include "Core\IEventListener.h"
#include "Core\GameSystem.h"

using namespace Windows::Networking;
using namespace Windows::Networking::Sockets;
using namespace Windows::Storage::Streams;

using namespace Microsoft::WRL;
using namespace PinnedDownClient::Core;

namespace PinnedDownClient
{
	namespace Systems
	{
		class NetworkSystem : public Core::GameSystem, public Core::IEventListener
		{
		public:
			NetworkSystem();

			void InitSystem(std::shared_ptr<GameInfrastructure> game);

			void InitSocket();

		private:
			StreamSocket^ clientSocket;
			HostName^ serverHost;
			DataWriter^ dataWriter;
			DataReader^ dataReader;
			bool connected = false;

			void OnEvent(Event & event);
		};
	}
}
