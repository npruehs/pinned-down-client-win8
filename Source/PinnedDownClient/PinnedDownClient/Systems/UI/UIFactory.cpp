#include "pch.h"
#include "UIFactory.h"

#include "Components\BoundsComponent.h"
#include "Components\ColorComponent.h"
#include "Components\FontComponent.h"
#include "Components\ScreenPositionComponent.h"
#include "Components\SpriteComponent.h"
#include "Components\TextAlignmentComponent.h"
#include "Components\TextComponent.h"
#include "Components\UIPanelComponent.h"

#include "Events\EntityInitializedEvent.h"

#include "Math\Vector2F.h"

using namespace PinnedDownClient::Components;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Math;
using namespace PinnedDownClient::Systems::UI;


UIFactory::UIFactory(std::shared_ptr<GameInfrastructure> game)
{
	this->game = game;
}

int UIFactory::CreateLabel(VerticalAnchor top, HorizontalAnchor left)
{
	return this->CreateLabel(top, left, 0);
}

int UIFactory::CreateLabel(VerticalAnchor top, HorizontalAnchor left, int anchorTarget)
{
	return this->CreateLabel(top, left, anchorTarget, std::wstring());
}

int UIFactory::CreateLabel(VerticalAnchor top, HorizontalAnchor left, int anchorTarget, std::wstring text)
{
	int entityId = this->game->entityManager->CreateEntity();

	auto boundsComponent = std::make_shared<BoundsComponent>();
	this->game->entityManager->AddComponent(entityId, boundsComponent);

	auto colorComponent = std::make_shared<ColorComponent>();
	colorComponent->color = D2D1::ColorF(D2D1::ColorF::White);
	this->game->entityManager->AddComponent(entityId, colorComponent);

	auto fontComponent = std::make_shared<FontComponent>();
	this->game->entityManager->AddComponent(entityId, fontComponent);

	auto screenPositionComponent = std::make_shared<ScreenPositionComponent>();
	this->game->entityManager->AddComponent(entityId, screenPositionComponent);

	auto textComponent = std::make_shared<TextComponent>();
	textComponent->text = text;
	this->game->entityManager->AddComponent(entityId, textComponent);

	auto textAlignmentComponent = std::make_shared<TextAlignmentComponent>();
	this->game->entityManager->AddComponent(entityId, textAlignmentComponent);

	this->AddAnchor(entityId, top, left, anchorTarget);

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

int UIFactory::CreateSprite(std::wstring spriteName)
{
	int entityId = this->game->entityManager->CreateEntity();

	auto screenPositionComponent = std::make_shared<ScreenPositionComponent>();
	this->game->entityManager->AddComponent(entityId, screenPositionComponent);

	this->AddSprite(entityId, spriteName);

	auto entityInitializedEvent = std::shared_ptr<Events::EntityInitializedEvent>(new Events::EntityInitializedEvent(entityId));
	this->game->eventManager->QueueEvent(entityInitializedEvent);

	return entityId;
}

int UIFactory::CreateSprite(std::wstring spriteName, VerticalAnchor top, HorizontalAnchor left)
{
	return this->CreateSprite(spriteName, top, left, 0);
}

int UIFactory::CreateSprite(std::wstring spriteName, VerticalAnchor top, HorizontalAnchor left, int anchorTarget)
{
	int entityId = this->game->entityManager->CreateEntity();

	auto screenPositionComponent = std::make_shared<ScreenPositionComponent>();
	this->game->entityManager->AddComponent(entityId, screenPositionComponent);

	this->AddSprite(entityId, spriteName);
	this->AddAnchor(entityId, top, left, anchorTarget);

	auto entityInitializedEvent = std::shared_ptr<Events::EntityInitializedEvent>(new Events::EntityInitializedEvent(entityId));
	this->game->eventManager->QueueEvent(entityInitializedEvent);

	return entityId;
}

void UIFactory::AddAnchor(int entityId, VerticalAnchor top, HorizontalAnchor left)
{
	this->AddAnchor(entityId, top, left, 0);
}

void UIFactory::AddAnchor(int entityId, VerticalAnchor top, HorizontalAnchor left, int anchorTarget)
{
	auto anchorComponent = std::make_shared<UIAnchorComponent>();
	anchorComponent->left = left;
	anchorComponent->top = top;
	anchorComponent->target = anchorTarget;
	this->game->entityManager->AddComponent(entityId, anchorComponent);
}

void UIFactory::AddSprite(int entityId, std::wstring spriteName)
{
	auto spriteComponent = std::make_shared<SpriteComponent>();
	spriteComponent->sprite = this->game->resourceManager->GetResource<BitmapResourceHandle>(spriteName.c_str());
	
	auto boundsComponent = std::make_shared<BoundsComponent>();
	auto bitmapSize = spriteComponent->sprite->bitmap->GetPixelSize();
	boundsComponent->bounds = Vector2F(static_cast<float>(bitmapSize.width), static_cast<float>(bitmapSize.height));
	this->game->entityManager->AddComponent(entityId, boundsComponent);

	this->game->entityManager->AddComponent(entityId, spriteComponent);
}
