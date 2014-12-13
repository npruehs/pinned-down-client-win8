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
	CALL_EVENT_HANDLER(CardSelectedEvent);
	CALL_EVENT_HANDLER(CardDeselectedEvent);
	CALL_EVENT_HANDLER(RenderTargetChangedEvent);
}

EVENT_HANDLER_DEFINITION(CardSelectionHighlightSystem, CardSelectedEvent)
{
	auto selectedCard = data.entity;
	auto selectedCardUiComponent = this->game->entityManager->GetComponent<CardUIComponent>(selectedCard, CardUIComponent::CardUIComponentType);

	// Add highlight.
	this->currentHighlight = this->uiFactory->CreateSprite("Assets/CardHighlight.png");
	this->uiFactory->SetAnchor(this->currentHighlight, VerticalAnchor(VerticalAnchorType::VerticalCenter, 0.0f), HorizontalAnchor(HorizontalAnchorType::HorizontalCenter, 0.0f), selectedCardUiComponent->background);
	this->uiFactory->SetDepth(this->currentHighlight, -1);
	this->uiFactory->FinishUIWidget(this->currentHighlight);
}

EVENT_HANDLER_DEFINITION(CardSelectionHighlightSystem, CardDeselectedEvent)
{
	this->game->entityManager->RemoveEntity(this->currentHighlight);
}

EVENT_HANDLER_DEFINITION(CardSelectionHighlightSystem, RenderTargetChangedEvent)
{
	this->d2dContext = data.d2dContext;

	this->LoadResources();
}
