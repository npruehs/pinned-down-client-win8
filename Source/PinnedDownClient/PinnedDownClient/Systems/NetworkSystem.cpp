#include "pch.h"

#include "Game.h"
#include "Event.h"

#include "Events\DisconnectedFromServerEvent.h"
#include "Events\LoginErrorEvent.h"
#include "Events\LoginSuccessEvent.h"

#include "Systems\NetworkSystem.h"

#include "Util\StringUtils.h"

#include "PinnedDownNet.h"

using namespace concurrency;

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems;

using namespace Windows::Networking;
using namespace Windows::Networking::Sockets;


using namespace Windows::Storage::Streams;


#define PINNED_DOWN_SERVER_HOST "localhost"
//#define PINNED_DOWN_SERVER_HOST "pinneddown.cloudapp.net"


NetworkSystem::NetworkSystem()
{
}

void NetworkSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, ConnectToServerAction::ConnectToServerActionType);
}

void NetworkSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(ConnectToServerAction);
}

EVENT_HANDLER_DEFINITION(NetworkSystem, ConnectToServerAction)
{
	this->InitSocket();
}

void NetworkSystem::InitSocket()
{
	this->clientSocket = ref new StreamSocket();
	this->serverHost = ref new HostName(PINNED_DOWN_SERVER_HOST);

	auto storeTask = create_task(this->clientSocket->ConnectAsync(serverHost, PINNED_DOWN_SERVER_PORT));

	storeTask.then([this](void)
	{
#ifdef REQUIRES_AUTH
		auto client = PinnedDownClient::Services::PinnedDownMobileService::GetClient();
		return client.login(azure::mobile::facebook);
	}).then([this](azure::mobile::user authUser)
	{
		if (authUser.is_authenticated())
		{
			this->user = authUser;
#endif
			this->connected = true;

			// Create the DataWriter object backed by the socket stream.  When dataWriter is deleted, it will also close the underlying stream.
			this->dataWriter = ref new DataWriter(this->clientSocket->OutputStream);
			dataWriter->UnicodeEncoding = UnicodeEncoding::Utf8;
			dataWriter->ByteOrder = ByteOrder::LittleEndian;
			this->clientActionWriter = std::make_shared<ClientActionWriter>(this->dataWriter);
			this->clientActionDispatcher = std::make_shared<ClientActionDispatcher>(this->game, this->clientActionWriter);

			this->dataReader = ref new DataReader(this->clientSocket->InputStream);
			dataReader->UnicodeEncoding = UnicodeEncoding::Utf8;
			dataReader->ByteOrder = ByteOrder::LittleEndian;
			this->serverEventReader = std::make_shared<ServerEventReader>(this->dataReader);

			// Start receive loop.
			this->RecvPacketLoop();
#ifdef REQUIRES_AUTH
		}
		else
		{
			// Notify listeners.
			auto loginErrorEvent = std::make_shared<LoginErrorEvent>("You must authenticate yourself in order to play Pinned Down.");
			this->game->eventManager->QueueEvent(loginErrorEvent);
		}
#endif
	}).then([this](task<void> t)
	{
		try
		{
			// Check for errors.
			t.get();
		}
		catch (Platform::Exception^ e)
		{
			// Notify listeners.
			auto loginErrorEvent = std::make_shared<LoginErrorEvent>("Error_UnableToConntect");
			this->game->eventManager->QueueEvent(loginErrorEvent);
		}
#ifdef REQUIRES_AUTH
		catch (azure::mobile::mobile_exception&)
		{
			// Notify listeners.
			auto loginErrorEvent = std::make_shared<LoginErrorEvent>("Error_AuthenticationRequired");
			this->game->eventManager->QueueEvent(loginErrorEvent);
		}
#endif
	});
}

void NetworkSystem::RecvPacketLoop()
{
	if (this->connected)
	{
		auto loadTask = create_task(this->dataReader->LoadAsync(4));

		loadTask.then([this](unsigned int bytesLoaded)
		{
			int packetSize = this->dataReader->ReadInt32();
			return this->dataReader->LoadAsync(packetSize);
		}).then([this](unsigned int bytesLoaded)
		{
			try
			{
				// Read server event.
				auto serverEvent = this->serverEventReader->ReadServerEvent(bytesLoaded);

				if (serverEvent != nullptr)
				{
					// Pass to game.
					this->game->eventManager->QueueEvent(serverEvent);
				}
				
				// Wait for next packet.
				this->RecvPacketLoop();
			}
			catch (Platform::Exception^ e)
			{
				OutputDebugString(e->Message->Data());
			}
		}).then([this](concurrency::task<void> t)
		{
			try {
				t.get();
				// .get() didn't throw, so we succeeded.
			}
			catch (Platform::Exception^ e) {
				OutputDebugString(e->Message->Data());
				this->connected = false;

				// Notify listeners.
				auto disconnectedFromServerEvent = std::make_shared<DisconnectedFromServerEvent>();
				this->game->eventManager->QueueEvent(disconnectedFromServerEvent);
			}
		});;
	}
}
