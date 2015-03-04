#include "pch.h"
#include "Event.h"
#include "ServerEventReader.h"

#include "Events\CardAssignedEvent.h"
#include "Events\CardCreatedEvent.h"
#include "Events\CardRemovedEvent.h"
#include "Events\CardStateChangedEvent.h"
#include "Events\CardUnassignedEvent.h"
#include "Events\CoveredDistanceChangedEvent.h"
#include "Events\DefeatEvent.h"
#include "Events\EffectPlayedEvent.h"
#include "Events\ErrorMessageEvent.h"
#include "Events\FightResolvedEvent.h"
#include "Events\GameStartedEvent.h"
#include "Events\LoginSuccessEvent.h"
#include "Events\PlayerAddedEvent.h"
#include "Events\PlayerReadyStateResetEvent.h"
#include "Events\PowerChangedEvent.h"
#include "Events\ShipDamagedEvent.h"
#include "Events\StarshipPlayedEvent.h"
#include "Events\StructureChangedEvent.h"
#include "Events\ThreatChangedEvent.h"
#include "Events\TurnPhaseChangedEvent.h"
#include "Events\VictoryEvent.h"

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

	if (hashedEventType == CardAssignedEvent::CardAssignedEventType)
	{
		auto cardAssignedEvent = std::make_shared<CardAssignedEvent>();
		cardAssignedEvent->Deserialize(in);
		return cardAssignedEvent;
	}
	else if (hashedEventType == CardCreatedEvent::CardCreatedEventType)
	{
		auto cardCreatedEvent = std::make_shared<CardCreatedEvent>();
		cardCreatedEvent->Deserialize(in);
		return cardCreatedEvent;
	}
	else if(hashedEventType == CardRemovedEvent::CardRemovedEventType)
	{
		auto cardRemovedEvent = std::make_shared<CardRemovedEvent>();
		cardRemovedEvent->Deserialize(in);
		return cardRemovedEvent;
	}
	else if (hashedEventType == CardStateChangedEvent::CardStateChangedEventType)
	{
		auto cardStateChangedEvent = std::make_shared<CardStateChangedEvent>();
		cardStateChangedEvent->Deserialize(in);
		return cardStateChangedEvent;
	}
	else if (hashedEventType == CardUnassignedEvent::CardUnassignedEventType)
	{
		auto cardUnassignedEvent = std::make_shared<CardUnassignedEvent>();
		cardUnassignedEvent->Deserialize(in);
		return cardUnassignedEvent;
	}
	else if (hashedEventType == CoveredDistanceChangedEvent::CoveredDistanceChangedEventType)
	{
		auto coveredDistanceChangedEvent = std::make_shared<CoveredDistanceChangedEvent>();
		coveredDistanceChangedEvent->Deserialize(in);
		return coveredDistanceChangedEvent;
	}
	else if (hashedEventType == DefeatEvent::DefeatEventType)
	{
		auto defeatEvent = std::make_shared<DefeatEvent>();
		defeatEvent->Deserialize(in);
		return defeatEvent;
	}
	else if (hashedEventType == EffectPlayedEvent::EffectPlayedEventType)
	{
		auto effectPlayedEvent = std::make_shared<EffectPlayedEvent>();
		effectPlayedEvent->Deserialize(in);
		return effectPlayedEvent;
	}
	else if (hashedEventType == ErrorMessageEvent::ErrorMessageEventType)
	{
		auto errorMessageEvent = std::make_shared<ErrorMessageEvent>();
		errorMessageEvent->Deserialize(in);
		return errorMessageEvent;
	}
	else if (hashedEventType == FightResolvedEvent::FightResolvedEventType)
	{
		auto fightResolvedEvent = std::make_shared<FightResolvedEvent>();
		fightResolvedEvent->Deserialize(in);
		return fightResolvedEvent;
	}
	else if (hashedEventType == GameStartedEvent::GameStartedEventType)
	{
		auto gameStartedEvent = std::make_shared<GameStartedEvent>();
		gameStartedEvent->Deserialize(in);
		return gameStartedEvent;
	}
	else if (hashedEventType == LoginSuccessEvent::LoginSuccessEventType)
	{
		auto loginSuccessEvent = std::make_shared<LoginSuccessEvent>();
		loginSuccessEvent->Deserialize(in);
		return loginSuccessEvent;
	}
	else if (hashedEventType == PlayerAddedEvent::PlayerAddedEventType)
	{
		auto playerAddedEvent = std::make_shared<PlayerAddedEvent>();
		playerAddedEvent->Deserialize(in);
		return playerAddedEvent;
	}
	else if (hashedEventType == PlayerReadyStateResetEvent::PlayerReadyStateResetEventType)
	{
		auto playerReadyStateResetEvent = std::make_shared<PlayerReadyStateResetEvent>();
		playerReadyStateResetEvent->Deserialize(in);
		return playerReadyStateResetEvent;
	}
	else if (hashedEventType == PowerChangedEvent::PowerChangedEventType)
	{
		auto powerChangedEvent = std::make_shared<PowerChangedEvent>();
		powerChangedEvent->Deserialize(in);
		return powerChangedEvent;
	}
	else if (hashedEventType == ShipDamagedEvent::ShipDamagedEventType)
	{
		auto shipDamagedEvent = std::make_shared<ShipDamagedEvent>();
		shipDamagedEvent->Deserialize(in);
		return shipDamagedEvent;
	}
	else if (hashedEventType == StarshipPlayedEvent::StarshipPlayedEventType)
	{
		auto starshipPlayedEvent = std::make_shared<StarshipPlayedEvent>();
		starshipPlayedEvent->Deserialize(in);
		return starshipPlayedEvent;
	}
	else if (hashedEventType == StructureChangedEvent::StructureChangedEventType)
	{
		auto structureChangedEvent = std::make_shared<StructureChangedEvent>();
		structureChangedEvent->Deserialize(in);
		return structureChangedEvent;
	}
	else if (hashedEventType == ThreatChangedEvent::ThreatChangedEventType)
	{
		auto threatChangedEvent = std::make_shared<ThreatChangedEvent>();
		threatChangedEvent->Deserialize(in);
		return threatChangedEvent;
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

	return nullptr;
}