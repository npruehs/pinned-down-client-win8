#include "pch.h"
#include "ClientActionWriter.h"

using namespace concurrency;

using namespace PinnedDownClient::Systems::Network;


ClientActionWriter::ClientActionWriter(DataWriter^ dataWriter)
{
	this->dataWriter = dataWriter;
}

void ClientActionWriter::WriteClientAction(Event& clientAction)
{
	// Convert event to bytes.
	std::ostrstream	out;

	out << clientAction.GetEventType().getString() << " ";
	clientAction.Serialize(out);
	out << "\r\n";

	auto buffer = out.rdbuf()->str();
	int count = out.pcount();

	BYTE* byteBuffer = reinterpret_cast<BYTE*>(buffer);
	UINT byteCount = (UINT)count;

	// Write bytes.
	this->dataWriter->WriteInt32(count);
	this->dataWriter->WriteBytes(ref new Platform::Array<BYTE>(byteBuffer, byteCount));

	// Call StoreAsync method to store the data to the backing stream.
	auto storeTask = create_task(dataWriter->StoreAsync());

	storeTask.then([this](unsigned int bytesStored)
	{
		return this->dataWriter->FlushAsync();
	});
}