#include "pch.h"

#include "Game.h"
#include "Event.h"

#include "Components\CardUIComponent.h"

#include "Resources\PinnedDownResourceManager.h"

#include "Systems\CardSelectionHighlightSystem.h"


using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Resources;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Util;


CardSelectionHighlightSystem::CardSelectionHighlightSystem()
{
}

void CardSelectionHighlightSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	this->uiFactory = std::make_shared<UIFactory>(game);

	this->game->eventManager->AddListener(this, CardSelectedEvent::CardSelectedEventType);
	this->game->eventManager->AddListener(this, CardDeselectedEvent::CardDeselectedEventType);
	this->game->eventManager->AddListener(this, RenderTargetChangedEvent::RenderTargetChangedEventType);
}

void CardSelectionHighlightSystem::LoadResources()
{
	auto resourceManager = static_cast<PinnedDownResourceManager*>(this->game->resourceManager.get());

	resourceManager->LoadBitmapFromFile(
		d2dContext.Get(),
		L"Assets/CardHighlight.png"
		);
}

void CardSelectionHighlightSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == CardSelectedEvent::CardSelectedEventType)
	{
		CardSelectedEvent& cardSelectedEvent = static_cast<CardSelectedEvent&>(newEvent);
		this->OnCardSelected(cardSelectedEvent);
	}
	else if (newEvent.GetEventType() == CardDeselectedEvent::CardDeselectedEventType)
	{
		CardDeselectedEvent& cardDeselectedEvent = static_cast<CardDeselectedEvent&>(newEvent);
		this->OnCardDeselected(cardDeselectedEvent);
	}
	else if (newEvent.GetEventType() == RenderTargetChangedEvent::RenderTargetChangedEventType)
	{
		auto renderTargetChangedEvent = static_cast<RenderTargetChangedEvent&>(newEvent);
		this->OnRenderTargetChanged(renderTargetChangedEvent);
	}
}

void CardSelectionHighlightSystem::OnCardSelected(CardSelectedEvent& cardSelectedEvent)
{
	auto selectedCard = cardSelectedEvent.entity;
	auto selectedCardUiComponent = this->game->entityManager->GetComponent<CardUIComponent>(selectedCard, CardUIComponent::CardUIComponentType);

	// Add highlight.
	this->currentHighlight = this->uiFactory->CreateSprite("Assets/CardHighlight.png");
	this->uiFactory->SetAnchor(this->currentHighlight, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), selectedCardUiComponent->background);
	this->uiFactory->SetDepth(this->currentHighlight, -1);
	this->uiFactory->FinishUIWidget(this->currentHighlight);
}

void CardSelectionHighlightSystem::OnCardDeselected(CardDeselectedEvent& cardDeselectedEvent)
{
	this->game->entityManager->RemoveEntity(this->currentHighlight);
}

void CardSelectionHighlightSystem::OnRenderTargetChanged(RenderTargetChangedEvent& renderTargetChangedEvent)
{
	this->d2dContext = renderTargetChangedEvent.d2dContext;

	this->LoadResources();
}
