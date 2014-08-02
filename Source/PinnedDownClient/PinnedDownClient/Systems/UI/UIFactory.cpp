#include "pch.h"
#include "UIFactory.h"

#include "Components\BoundsComponent.h"
#include "Components\ColorComponent.h"
#include "Components\DepthComponent.h"
#include "Components\FontComponent.h"
#include "Components\ScreenPositionComponent.h"
#include "Components\SpriteComponent.h"
#include "Components\TappableComponent.h"
#include "Components\TextAlignmentComponent.h"
#include "Components\TextComponent.h"
#include "Components\UIPanelComponent.h"
#include "Components\UIWidgetComponent.h"
#include "Components\VisibilityComponent.h"

#include "Events\EntityInitializedEvent.h"

#include "Math\Vector2F.h"

using namespace PinnedDownClient::Components;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Math;
using namespace PinnedDownClient::Systems::UI;


UIFactory::UIFactory(PinnedDownCore::Game* game)
{
	this->game = game;
}

int UIFactory::CreateLabel(std::wstring text)
{
	int entityId = this->game->entityManager->CreateEntity();

	auto boundsComponent = std::make_shared<BoundsComponent>();
	this->game->entityManager->AddComponent(entityId, boundsComponent);

	auto colorComponent = std::make_shared<ColorComponent>();
	colorComponent->color = D2D1::ColorF(D2D1::ColorF::White);
	this->game->entityManager->AddComponent(entityId, colorComponent);

	auto depthComponent = std::make_shared<DepthComponent>();
	this->game->entityManager->AddComponent(entityId, depthComponent);

	auto fontComponent = std::make_shared<FontComponent>();
	this->game->entityManager->AddComponent(entityId, fontComponent);

	auto screenPositionComponent = std::make_shared<ScreenPositionComponent>();
	this->game->entityManager->AddComponent(entityId, screenPositionComponent);

	auto textComponent = std::make_shared<TextComponent>();
	textComponent->text = text;
	this->game->entityManager->AddComponent(entityId, textComponent);

	auto textAlignmentComponent = std::make_shared<TextAlignmentComponent>();
	this->game->entityManager->AddComponent(entityId, textAlignmentComponent);

	auto anchorComponent = std::make_shared<UIAnchorComponent>();
	this->game->entityManager->AddComponent(entityId, anchorComponent);

	auto widgetComponent = std::make_shared<UIWidgetComponent>();
	this->game->entityManager->AddComponent(entityId, widgetComponent);

	auto visibilityComponent = std::make_shared<VisibilityComponent>();
	this->game->entityManager->AddComponent(entityId, visibilityComponent);

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

	auto depthComponent = std::make_shared<DepthComponent>();
	this->game->entityManager->AddComponent(entityId, depthComponent);

	auto anchorComponent = std::make_shared<UIAnchorComponent>();
	this->game->entityManager->AddComponent(entityId, anchorComponent);

	auto visibilityComponent = std::make_shared<VisibilityComponent>();
	this->game->entityManager->AddComponent(entityId, visibilityComponent);

	return entityId;
}

int UIFactory::CreateSprite(std::wstring spriteName)
{
	int entityId = this->game->entityManager->CreateEntity();

	auto screenPositionComponent = std::make_shared<ScreenPositionComponent>();
	this->game->entityManager->AddComponent(entityId, screenPositionComponent);

	auto depthComponent = std::make_shared<DepthComponent>();
	this->game->entityManager->AddComponent(entityId, depthComponent);

	auto spriteComponent = std::make_shared<SpriteComponent>();
	spriteComponent->sprite = this->game->resourceManager->GetResource<BitmapResourceHandle>(spriteName.c_str());
	this->game->entityManager->AddComponent(entityId, spriteComponent);

	auto boundsComponent = std::make_shared<BoundsComponent>();
	auto bitmapSize = spriteComponent->sprite->bitmap->GetPixelSize();
	boundsComponent->bounds = Vector2F(static_cast<float>(bitmapSize.width), static_cast<float>(bitmapSize.height));
	this->game->entityManager->AddComponent(entityId, boundsComponent);

	auto anchorComponent = std::make_shared<UIAnchorComponent>();
	this->game->entityManager->AddComponent(entityId, anchorComponent);

	auto widgetComponent = std::make_shared<UIWidgetComponent>();
	this->game->entityManager->AddComponent(entityId, widgetComponent);

	auto visibilityComponent = std::make_shared<VisibilityComponent>();
	this->game->entityManager->AddComponent(entityId, visibilityComponent);

	return entityId;
}

void UIFactory::SetAnchor(int entityId, VerticalAnchor top, HorizontalAnchor left)
{
	auto anchorComponent = this->game->entityManager->GetComponent<UIAnchorComponent>(entityId, UIAnchorComponent::UIAnchorComponentType);
	anchorComponent->top = top;
	anchorComponent->left = left;
}

void UIFactory::SetAnchor(int entityId, VerticalAnchor top, HorizontalAnchor left, int anchorTarget)
{
	auto anchorComponent = this->game->entityManager->GetComponent<UIAnchorComponent>(entityId, UIAnchorComponent::UIAnchorComponentType);
	anchorComponent->top = top;
	anchorComponent->left = left;
	anchorComponent->target = anchorTarget;
}

void UIFactory::SetDepth(int entityId, int depth)
{
	auto depthComponent = this->game->entityManager->GetComponent<DepthComponent>(entityId, DepthComponent::DepthComponentType);
	depthComponent->depth = depth;
}

void UIFactory::SetPanel(int entityId, int panelId)
{
	auto widgetComponent = this->game->entityManager->GetComponent<UIWidgetComponent>(entityId, UIWidgetComponent::UIWidgetComponentType);
	widgetComponent->panel = panelId;
}

void UIFactory::SetVisible(int entityId, bool visible)
{
	auto visibilityComponent = this->game->entityManager->GetComponent<VisibilityComponent>(entityId, VisibilityComponent::VisibilityComponentType);
	visibilityComponent->visible = visible;
}

void UIFactory::SetTappable(int entityId)
{
	auto tappableComponent = std::make_shared<TappableComponent>();
	this->game->entityManager->AddComponent(entityId, tappableComponent);
}

void UIFactory::FinishUIWidget(int entityId)
{
	auto entityInitializedEvent = std::shared_ptr<Events::EntityInitializedEvent>(new Events::EntityInitializedEvent(entityId));
	this->game->eventManager->QueueEvent(entityInitializedEvent);
}