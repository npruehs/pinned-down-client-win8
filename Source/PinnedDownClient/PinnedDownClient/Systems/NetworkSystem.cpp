#include "pch.h"

#include "Game.h"
#include "Event.h"

#include "Events\LoginErrorEvent.h"
#include "Events\LoginSuccessEvent.h"
#include "Events\ScreenChangedEvent.h"
#include "Events\PointerPressedEvent.h"

#include "Systems\NetworkSystem.h"


using namespace concurrency;

using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems;

using namespace Windows::Networking;
using namespace Windows::Networking::Sockets;


using namespace Windows::Storage::Streams;


#define PINNED_DOWN_SERVER_HOST "localhost"
#define PINNED_DOWN_SERVER_PORT "27015"


NetworkSystem::NetworkSystem()
{
}

void NetworkSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, ScreenChangedEvent::ScreenChangedEventType);
	this->game->eventManager->AddListener(this, PointerPressedEvent::PointerPressedEventType);
}

void NetworkSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == ScreenChangedEvent::ScreenChangedEventType)
	{
		auto screenChangedEvent = static_cast<ScreenChangedEvent&>(newEvent);
		this->OnScreenChanged(screenChangedEvent);
	}
	else if (newEvent.GetEventType() == PointerPressedEvent::PointerPressedEventType)
	{
		auto pointerPressedEvent = static_cast<PointerPressedEvent&>(newEvent);
		this->OnPointerPressed(pointerPressedEvent);
	}
}

void NetworkSystem::OnPointerPressed(PointerPressedEvent& pointerPressedEvent)
{
	this->SendPacket();
}

void NetworkSystem::OnScreenChanged(ScreenChangedEvent& screenChangedEvent)
{
	if (screenChangedEvent.newScreen == ScreenName::Login)
	{
		this->InitSocket();
	}
}

void NetworkSystem::InitSocket()
{
	this->clientSocket = ref new StreamSocket();
	this->serverHost = ref new HostName(PINNED_DOWN_SERVER_HOST);

	auto storeTask = create_task(this->clientSocket->ConnectAsync(serverHost, PINNED_DOWN_SERVER_PORT));

	storeTask.then([this](void)
	{
		this->connected = true;

		// Create the DataWriter object backed by the socket stream.  When dataWriter is deleted, it will also close the underlying stream.
		this->dataWriter = ref new DataWriter(this->clientSocket->OutputStream);
		dataWriter->UnicodeEncoding = UnicodeEncoding::Utf8;
		dataWriter->ByteOrder = ByteOrder::LittleEndian;

		this->dataReader = ref new DataReader(this->clientSocket->InputStream);
		dataReader->UnicodeEncoding = UnicodeEncoding::Utf8;
		dataReader->ByteOrder = ByteOrder::LittleEndian;

		// Notify listeners.
		auto loginSuccessEvent = std::make_shared<LoginSuccessEvent>();
		this->game->eventManager->QueueEvent(loginSuccessEvent);

		// Start packet queue.
		this->RecvPacketQueue();
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
			auto errorMessage = e->Message->Data();
			auto loginErrorEvent = std::make_shared<LoginErrorEvent>(errorMessage);
			this->game->eventManager->QueueEvent(loginErrorEvent);
		}
	});
}

void NetworkSystem::RecvPacketQueue()
{
	if (this->connected)
	{
		auto loadTask = create_task(this->dataReader->LoadAsync(4));

		loadTask.then([this](unsigned int bytesLoaded)
		{
			try
			{
				//unsigned int packetType = this->dataReader->ReadUInt32();
				unsigned int dataSize = this->dataReader->ReadUInt32();

				OutputDebugString(std::to_wstring(dataSize).c_str());

				// TODO: Convert to while-loop.
				this->RecvPacketQueue();
			}
			catch (Platform::Exception^ e)
			{
				OutputDebugString(e->Message->Data());
			}
		});
	}
}

void NetworkSystem::SendPacket()
{
	if (!this->connected)
	{
		return;
	}

	// Get the UTF-8 string length.
	unsigned int len = this->dataWriter->MeasureString("test");

	dataWriter->WriteUInt32(len);
	dataWriter->WriteString("test");

	// Call StoreAsync method to store the data to the backing stream.
	auto storeTask = create_task(dataWriter->StoreAsync());

	storeTask.then([this](unsigned int bytesStored)
	{
		return this->dataWriter->FlushAsync();
	}).then([this, len](bool flushOp)
	{
		// Once we have written the contents successfully we load the stream.
		return this->dataReader->LoadAsync(len + 4);
	}).then([this](unsigned int bytesLoaded)
	{
		try
		{
			Platform::String^ readFromStream = "";

			// Keep reading until we consume the complete stream.
			while (this->dataReader->UnconsumedBufferLength > 0)
			{
				unsigned int bufferLength = this->dataReader->UnconsumedBufferLength;

				unsigned int bytesToRead = this->dataReader->ReadUInt32();
				bufferLength = this->dataReader->UnconsumedBufferLength;
				readFromStream += this->dataReader->ReadString(bytesToRead);
			}

			OutputDebugString(readFromStream->Data());
		}
		catch (Platform::Exception^ e)
		{
			OutputDebugString(e->Message->Data());
		}
	});
}
