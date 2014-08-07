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
	this->game->eventManager->AddListener(this, CardCopyCreatedEvent::CardCopyCreatedEventType);
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
	else if (newEvent.GetEventType() == CardCopyCreatedEvent::CardCopyCreatedEventType)
	{
		CardCopyCreatedEvent& cardCopyCreatedEvent = static_cast<CardCopyCreatedEvent&>(newEvent);
		this->OnCardCopyCreated(cardCopyCreatedEvent);
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

void CardLayoutSystem::OnCardCopyCreated(CardCopyCreatedEvent& cardCopyCreated)
{
	auto card = std::make_shared<Card>();
	card->cardEntity = cardCopyCreated.card;

	// Card background sprite.
	card->backgroundSprite = this->uiFactory->CreateSprite("Assets/BlueWingStarship.png");
	this->uiFactory->SetAnchor(card->backgroundSprite, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), 0);
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
}

void CardLayoutSystem::OnRenderTargetChanged(RenderTargetChangedEvent& renderTargetChangedEvent)
{
	this->d2dContext = renderTargetChangedEvent.d2dContext;

	this->LoadResources();
}