#include "pch.h"
#include "Event.h"
#include "ServerEventReader.h"

#include "Events\LoginSuccessEvent.h"
#include "Events\CoveredDistanceChangedEvent.h"

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
	else if (hashedEventType == CoveredDistanceChangedEvent::CoveredDistanceChangedEventType)
	{
		auto coveredDistanceChangedEvent = std::make_shared<CoveredDistanceChangedEvent>();
		coveredDistanceChangedEvent->Deserialize(in);
		return coveredDistanceChangedEvent;
	}

	return nullptr;
}