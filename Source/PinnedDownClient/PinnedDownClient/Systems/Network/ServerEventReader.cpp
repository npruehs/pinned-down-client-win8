#include "pch.h"
#include "ServerEventReader.h"

using namespace PinnedDownClient::Systems::Network;


ServerEventReader::ServerEventReader(DataReader^ dataReader)
{
	this->dataReader = dataReader;
}

ServerEvent ServerEventReader::ReadServerEvent()
{
	// Read event type.
	ServerEventType eventType = (ServerEventType)this->dataReader->ReadInt32();

	// Read event data.
	ServerEvent serverEvent = ServerEvent(eventType);

	switch (eventType)
	{
	case ServerEventType::LoginSuccess:
		OutputDebugString(L"LoginSuccess");
	}

	return serverEvent;
}