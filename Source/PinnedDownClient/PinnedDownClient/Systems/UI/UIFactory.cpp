#include "pch.h"
#include "UIFactory.h"

#include "Components\BoundsComponent.h"
#include "Components\ColorComponent.h"
#include "Components\FontComponent.h"
#include "Components\ScreenPositionComponent.h"
#include "Components\TextAlignmentComponent.h"
#include "Components\TextComponent.h"
#include "Components\UIPanelComponent.h"

#include "Events\EntityInitializedEvent.h"

using namespace PinnedDownClient::Components;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems::UI;

UIFactory::UIFactory(std::shared_ptr<GameInfrastructure> game)
{
	this->game = game;
}


int UIFactory::CreateLabel(VerticalAnchor top, HorizontalAnchor left)
{
	int entityId = this->game->entityManager->CreateEntity();

	auto colorComponent = std::make_shared<ColorComponent>();
	colorComponent->color = D2D1::ColorF(D2D1::ColorF::White);
	this->game->entityManager->AddComponent(entityId, colorComponent);

	auto fontComponent = std::make_shared<FontComponent>();
	this->game->entityManager->AddComponent(entityId, fontComponent);

	auto screenPositionComponent = std::make_shared<ScreenPositionComponent>();
	this->game->entityManager->AddComponent(entityId, screenPositionComponent);

	auto textComponent = std::make_shared<TextComponent>();
	this->game->entityManager->AddComponent(entityId, textComponent);

	auto textAlignmentComponent = std::make_shared<TextAlignmentComponent>();
	this->game->entityManager->AddComponent(entityId, textAlignmentComponent);

	auto anchorComponent = std::make_shared<UIAnchorComponent>();
	anchorComponent->left = left;
	anchorComponent->top = top;
	this->game->entityManager->AddComponent(entityId, anchorComponent);

	auto entityInitializedEvent = std::shared_ptr<Events::EntityInitializedEvent>(new Events::EntityInitializedEvent(entityId));
	this->game->eventManager->QueueEvent(entityInitializedEvent);

	return entityId;
}

int UIFactory::CreatePanel()
{
	int entityId = this->game->entityManager->CreateEntity();

	auto panelComponent = std::make_shared<UIPanelComponent>();
	this->game->entityManager->AddComponent(entityId, panelComponent);

	auto boundsComponent = std::make_shared<BoundsComponent>();
	this->game->entityManager->AddComponent(entityId, boundsComponent);

	auto screenPositionComponent = std::make_shared<ScreenPositionComponent>();
	this->game->entityManager->AddComponent(entityId, screenPositionComponent);

	auto entityInitializedEvent = std::shared_ptr<Events::EntityInitializedEvent>(new Events::EntityInitializedEvent(entityId));
	this->game->eventManager->QueueEvent(entityInitializedEvent);

	return entityId;
}