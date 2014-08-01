#include "pch.h"
#include "ClientActionWriter.h"

using namespace concurrency;

using namespace PinnedDownClient::Systems::Network;


ClientActionWriter::ClientActionWriter(DataWriter^ dataWriter)
{
	this->dataWriter = dataWriter;
}

ClientAction ClientActionWriter::WriteClientAction(ClientAction action)
{
	// Write event type.
	this->dataWriter->WriteInt32(action.actionType);

	// Write event data.

	// Call StoreAsync method to store the data to the backing stream.
	auto storeTask = create_task(dataWriter->StoreAsync());

	storeTask.then([this](unsigned int bytesStored)
	{
		return this->dataWriter->FlushAsync();
	});
}