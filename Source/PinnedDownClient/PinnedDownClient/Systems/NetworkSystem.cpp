#include "pch.h"

#include "Core\Event.h"
#include "Systems\NetworkSystem.h"
#include "Events\PointerPressedEvent.h"


using namespace concurrency;

using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Util;

using namespace Windows::Networking;
using namespace Windows::Networking::Sockets;


using namespace Windows::Storage::Streams;


NetworkSystem::NetworkSystem()
{
}

void NetworkSystem::InitSystem(std::shared_ptr<PinnedDownClient::GameInfrastructure> game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(std::shared_ptr<IEventListener>(this), PointerPressedEvent::PointerPressedEventType);

	this->InitSocket();
}

void NetworkSystem::InitSocket()
{
	this->clientSocket = ref new StreamSocket();
	this->serverHost = ref new HostName("localhost");

	auto storeTask = create_task(this->clientSocket->ConnectAsync(serverHost, "27015"));

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
	});


}

void NetworkSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == PointerPressedEvent::PointerPressedEventType)
	{
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
}
