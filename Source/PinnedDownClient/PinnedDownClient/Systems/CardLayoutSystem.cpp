#include "pch.h"

#include "Game.h"
#include "Event.h"

#include "Components\AffiliationComponent.h"
#include "Components\CardComponent.h"
#include "Components\PowerComponent.h"
#include "Components\ThreatComponent.h"

#include "Resources\PinnedDownResourceManager.h"

#include "Systems\CardLayoutSystem.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Resources;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Util;



CardLayoutSystem::CardLayoutSystem()
{
}

void CardLayoutSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	this->uiFactory = std::make_shared<UIFactory>(game);

	this->game->eventManager->AddListener(this, CardIdMappingCreatedEvent::CardIdMappingCreatedEventType);
	this->game->eventManager->AddListener(this, CardCreatedEvent::CardCreatedEventType);
	this->game->eventManager->AddListener(this, CardRemovedEvent::CardRemovedEventType);
	this->game->eventManager->AddListener(this, RenderTargetChangedEvent::RenderTargetChangedEventType);
}

void CardLayoutSystem::LoadResources()
{
	auto resourceManager = static_cast<PinnedDownResourceManager*>(this->game->resourceManager.get());

	resourceManager->LoadBitmapFromFile(
		d2dContext.Get(),
		L"Assets/BlueWingStarship.png"
		);
}

void CardLayoutSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == CardIdMappingCreatedEvent::CardIdMappingCreatedEventType)
	{
		CardIdMappingCreatedEvent& cardIdMappingCreatedEvent = static_cast<CardIdMappingCreatedEvent&>(newEvent);
		this->OnCardIdMappingCreated(cardIdMappingCreatedEvent);
	}
	else if (newEvent.GetEventType() == CardCreatedEvent::CardCreatedEventType)
	{
		CardCreatedEvent& cardCreatedEvent = static_cast<CardCreatedEvent&>(newEvent);
		this->OnCardCreated(cardCreatedEvent);
	}
	else if (newEvent.GetEventType() == CardRemovedEvent::CardRemovedEventType)
	{
		CardRemovedEvent& cardRemovedEvent = static_cast<CardRemovedEvent&>(newEvent);
		this->OnCardRemoved(cardRemovedEvent);
	}
	if (newEvent.GetEventType() == RenderTargetChangedEvent::RenderTargetChangedEventType)
	{
		auto renderTargetChangedEvent = static_cast<RenderTargetChangedEvent&>(newEvent);
		this->OnRenderTargetChanged(renderTargetChangedEvent);
	}
}

void CardLayoutSystem::OnCardIdMappingCreated(CardIdMappingCreatedEvent& cardIdMappingCreatedEvent)
{
	this->cardIdMapping = cardIdMappingCreatedEvent.cardIdMapping;
}

void CardLayoutSystem::OnCardCreated(CardCreatedEvent& cardCreatedEvent)
{
	auto card = std::make_shared<Card>();
	card->cardEntity = this->cardIdMapping->ServerToClientId(cardCreatedEvent.serverEntity);

	// Card background sprite.
	card->backgroundSprite = this->uiFactory->CreateSprite("Assets/BlueWingStarship.png");
	this->uiFactory->SetAnchor(card->backgroundSprite, VerticalAnchor(VerticalAnchorType::VerticalCenter, 200.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
	this->uiFactory->FinishUIWidget(card->backgroundSprite);

	// Name label.
	auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(card->cardEntity, CardComponent::CardComponentType);

	card->nameLabel = this->uiFactory->CreateLabel(L"Card " + std::to_wstring(cardComponent->cardIndex));
	this->uiFactory->SetAnchor(card->nameLabel, VerticalAnchor(VerticalAnchorType::Top, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), card->backgroundSprite);
	this->uiFactory->SetColor(card->nameLabel, D2D1::ColorF(D2D1::ColorF::Black));
	this->uiFactory->FinishUIWidget(card->nameLabel);

	// Card type label.
	card->cardTypeLabel = this->uiFactory->CreateLabel(L"Starship");
	this->uiFactory->SetAnchor(card->cardTypeLabel, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), card->backgroundSprite);
	this->uiFactory->SetColor(card->cardTypeLabel, D2D1::ColorF(D2D1::ColorF::Black));
	this->uiFactory->FinishUIWidget(card->cardTypeLabel);

	// Threat label.
	auto threatComponent = this->game->entityManager->GetComponent<ThreatComponent>(card->cardEntity, ThreatComponent::ThreatComponentType);

	card->threatLabel = this->uiFactory->CreateLabel(L"[" + std::to_wstring(threatComponent->threat) + L"]");
	this->uiFactory->SetAnchor(card->threatLabel, VerticalAnchor(VerticalAnchorType::Top, 0.0f), HorizontalAnchor(HorizontalAnchorType::Left, 0.0f), card->backgroundSprite);
	this->uiFactory->SetColor(card->threatLabel, D2D1::ColorF(D2D1::ColorF::Black));
	this->uiFactory->FinishUIWidget(card->threatLabel);

	// Power label.
	auto powerComponent = this->game->entityManager->GetComponent<PowerComponent>(card->cardEntity, PowerComponent::PowerComponentType);

	card->powerLabel = this->uiFactory->CreateLabel(L"Power " + std::to_wstring(powerComponent->power));
	this->uiFactory->SetAnchor(card->powerLabel, VerticalAnchor(VerticalAnchorType::Bottom, 0.0f), HorizontalAnchor(HorizontalAnchorType::Left, 0.0f), card->backgroundSprite);
	this->uiFactory->SetColor(card->powerLabel, D2D1::ColorF(D2D1::ColorF::Black));
	this->uiFactory->FinishUIWidget(card->powerLabel);

	// Add to list.
	this->cards.push_back(card);
}

void CardLayoutSystem::OnCardRemoved(CardRemovedEvent& cardRemovedEvent)
{
	auto clientEntity = this->cardIdMapping->ServerToClientId(cardRemovedEvent.serverEntity);

	for (auto iterator = this->cards.begin(); iterator != this->cards.end(); iterator++)
	{
		auto card = *iterator;

		if (card->cardEntity == clientEntity)
		{
			// Remove card.
			this->game->entityManager->RemoveEntity(card->backgroundSprite);
			this->game->entityManager->RemoveEntity(card->cardEntity);
			this->game->entityManager->RemoveEntity(card->cardTypeLabel);
			this->game->entityManager->RemoveEntity(card->nameLabel);
			this->game->entityManager->RemoveEntity(card->powerLabel);
			this->game->entityManager->RemoveEntity(card->threatLabel);

			this->cards.erase(iterator);
			break;
		}
	}
}

void CardLayoutSystem::OnRenderTargetChanged(RenderTargetChangedEvent& renderTargetChangedEvent)
{
	this->d2dContext = renderTargetChangedEvent.d2dContext;

	this->LoadResources();
}