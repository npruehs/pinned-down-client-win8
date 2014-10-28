#include "pch.h"

#include "Game.h"
#include "Event.h"

#include "Systems\CardIdMappingSystem.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Util;



CardIdMappingSystem::CardIdMappingSystem()
{
}

void CardIdMappingSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);
	
	// Setup card factory.
	this->cardFactory = std::make_shared<CardFactory>(this->game);

	// Register listener.
	this->game->eventManager->AddListener(this, CardCreatedEvent::CardCreatedEventType);
	this->game->eventManager->AddListener(this, EntityIdMappingCreatedEvent::EntityIdMappingCreatedEventType);
}

void CardIdMappingSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == CardCreatedEvent::CardCreatedEventType)
	{
		CardCreatedEvent& cardCreatedEvent = static_cast<CardCreatedEvent&>(newEvent);
		this->OnCardCreated(cardCreatedEvent);
	}
	if (newEvent.GetEventType() == EntityIdMappingCreatedEvent::EntityIdMappingCreatedEventType)
	{
		EntityIdMappingCreatedEvent& entityIdMappingCreatedEvent = static_cast<EntityIdMappingCreatedEvent&>(newEvent);
		this->OnEntityIdMappingCreated(entityIdMappingCreatedEvent);
	}
}

void CardIdMappingSystem::OnCardCreated(CardCreatedEvent& cardCreatedEvent)
{
	// Create card copy on client.
	Entity clientEntity = this->cardFactory->CreateCard(cardCreatedEvent.owner, cardCreatedEvent.setIndex, cardCreatedEvent.cardIndex, cardCreatedEvent.initialState);

	// Update mapping.
	this->entityIdMapping->MapEntityId(cardCreatedEvent.serverEntity, clientEntity);
}

void CardIdMappingSystem::OnEntityIdMappingCreated(EntityIdMappingCreatedEvent& entityIdMappingCreatedEvent)
{
	this->entityIdMapping = entityIdMappingCreatedEvent.entityIdMapping;
}
