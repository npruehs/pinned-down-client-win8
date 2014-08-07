#include "pch.h"
#include "Event.h"
#include "ServerEventReader.h"

#include "Events\CardCreatedEvent.h"
#include "Events\CardRemovedEvent.h"
#include "Events\LoginSuccessEvent.h"
#include "Events\CoveredDistanceChangedEvent.h"
#include "Events\TurnPhaseChangedEvent.h"
#include "Events\VictoryEvent.h"
#include "Events\DefeatEvent.h"

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

	if (hashedEventType == CardCreatedEvent::CardCreatedEventType)
	{
		auto cardCreatedEvent = std::make_shared<CardCreatedEvent>();
		cardCreatedEvent->Deserialize(in);
		return cardCreatedEvent;
	}
	if (hashedEventType == CardRemovedEvent::CardRemovedEventType)
	{
		auto cardRemovedEvent = std::make_shared<CardRemovedEvent>();
		cardRemovedEvent->Deserialize(in);
		return cardRemovedEvent;
	}
	else if (hashedEventType == LoginSuccessEvent::LoginSuccessEventType)
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
	else if (hashedEventType == TurnPhaseChangedEvent::TurnPhaseChangedEventType)
	{
		auto turnPhaseChangedEvent = std::make_shared<TurnPhaseChangedEvent>();
		turnPhaseChangedEvent->Deserialize(in);
		return turnPhaseChangedEvent;
	}
	else if (hashedEventType == VictoryEvent::VictoryEventType)
	{
		auto victoryEvent = std::make_shared<VictoryEvent>();
		victoryEvent->Deserialize(in);
		return victoryEvent;
	}
	else if (hashedEventType == DefeatEvent::DefeatEventType)
	{
		auto defeatEvent = std::make_shared<DefeatEvent>();
		defeatEvent->Deserialize(in);
		return defeatEvent;
	}

	return nullptr;
}