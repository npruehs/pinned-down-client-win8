#include "pch.h"
#include "Event.h"
#include "ServerEventReader.h"

#include "Events\LoginSuccessEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Systems::Network;


ServerEventReader::ServerEventReader(DataReader^ dataReader)
{
	this->dataReader = dataReader;
}

std::shared_ptr<Event> ServerEventReader::ReadServerEvent(int packetSize)
{
	auto bytes = ref new Platform::Array<BYTE>(packetSize);
	this->dataReader->ReadBytes(bytes);

	auto byteBuffer = bytes->Data;
	char* buffer = reinterpret_cast<char*>(byteBuffer);

	std::istrstream	in(buffer);

	char eventType[256];
	in >> eventType;

	HashedString hashedEventType = HashedString(eventType);

	if (hashedEventType == LoginSuccessEvent::LoginSuccessEventType)
	{
		auto loginSuccessEvent = std::make_shared<LoginSuccessEvent>();
		loginSuccessEvent->Deserialize(in);
		return loginSuccessEvent;
	}

	return nullptr;
}