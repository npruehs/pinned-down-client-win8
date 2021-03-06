#include "pch.h"
#include "UIFactory.h"

#include "Components\BoundsComponent.h"
#include "Components\ColorComponent.h"
#include "Components\DepthComponent.h"
#include "Components\FontComponent.h"
#include "Components\LocalizationComponent.h"
#include "Components\ScreenPositionComponent.h"
#include "Components\SpriteComponent.h"
#include "Components\TappableComponent.h"
#include "Components\TextAlignmentComponent.h"
#include "Components\TextComponent.h"
#include "Components\UIPanelComponent.h"
#include "Components\UIWidgetComponent.h"
#include "Components\VisibilityComponent.h"

#include "EntityInitializedEvent.h"

#include "Math\Vector2F.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Components;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Math;
using namespace PinnedDownClient::Systems::UI;


UIFactory::UIFactory(PinnedDownCore::Game* game)
{
	this->game = game;
}

Entity UIFactory::CreateLabel()
{
	return this->CreateLabel(L"");
}

Entity UIFactory::CreateLabel(std::wstring localizationKey)
{
	return this->CreateLabel(localizationKey, 0.0f);
}

Entity UIFactory::CreateLabel(std::wstring localizationKey, float maxWidth)
{
	Entity entity = this->game->entityManager->CreateEntity();

	auto boundsComponent = std::make_shared<BoundsComponent>();
	this->game->entityManager->AddComponent(entity, boundsComponent);

	auto colorComponent = std::make_shared<ColorComponent>();
	colorComponent->color = D2D1::ColorF(D2D1::ColorF::White);
	this->game->entityManager->AddComponent(entity, colorComponent);

	auto depthComponent = std::make_shared<DepthComponent>();
	this->game->entityManager->AddComponent(entity, depthComponent);

	auto fontComponent = std::make_shared<FontComponent>();
	this->game->entityManager->AddComponent(entity, fontComponent);

	auto localizationComponent = std::make_shared<LocalizationComponent>();
	localizationComponent->localizationKey = localizationKey;
	this->game->entityManager->AddComponent(entity, localizationComponent);

	auto screenPositionComponent = std::make_shared<ScreenPositionComponent>();
	this->game->entityManager->AddComponent(entity, screenPositionComponent);

	auto textComponent = std::make_shared<TextComponent>();
	this->game->entityManager->AddComponent(entity, textComponent);

	if (maxWidth > 0)
	{
		textComponent->maxWidth = maxWidth;
	}

	auto textAlignmentComponent = std::make_shared<TextAlignmentComponent>();
	this->game->entityManager->AddComponent(entity, textAlignmentComponent);

	auto anchorComponent = std::make_shared<UIAnchorComponent>();
	this->game->entityManager->AddComponent(entity, anchorComponent);

	auto widgetComponent = std::make_shared<UIWidgetComponent>();
	this->game->entityManager->AddComponent(entity, widgetComponent);

	auto visibilityComponent = std::make_shared<VisibilityComponent>();
	this->game->entityManager->AddComponent(entity, visibilityComponent);

	return entity;
}

Entity UIFactory::CreatePanel()
{
	Entity entity = this->game->entityManager->CreateEntity();

	auto panelComponent = std::make_shared<UIPanelComponent>();
	this->game->entityManager->AddComponent(entity, panelComponent);

	auto boundsComponent = std::make_shared<BoundsComponent>();
	this->game->entityManager->AddComponent(entity, boundsComponent);

	auto screenPositionComponent = std::make_shared<ScreenPositionComponent>();
	this->game->entityManager->AddComponent(entity, screenPositionComponent);

	auto depthComponent = std::make_shared<DepthComponent>();
	this->game->entityManager->AddComponent(entity, depthComponent);

	auto anchorComponent = std::make_shared<UIAnchorComponent>();
	this->game->entityManager->AddComponent(entity, anchorComponent);

	auto visibilityComponent = std::make_shared<VisibilityComponent>();
	this->game->entityManager->AddComponent(entity, visibilityComponent);

	return entity;
}

Entity UIFactory::CreateSprite(std::string spriteName)
{
	Entity entity = this->game->entityManager->CreateEntity();

	auto screenPositionComponent = std::make_shared<ScreenPositionComponent>();
	this->game->entityManager->AddComponent(entity, screenPositionComponent);

	auto depthComponent = std::make_shared<DepthComponent>();
	this->game->entityManager->AddComponent(entity, depthComponent);

	auto spriteComponent = std::make_shared<SpriteComponent>();
	spriteComponent->sprite = this->game->resourceManager->GetResource<BitmapResourceHandle>(spriteName);
	this->game->entityManager->AddComponent(entity, spriteComponent);

	auto boundsComponent = std::make_shared<BoundsComponent>();
	auto bitmapSize = spriteComponent->sprite->bitmap->GetPixelSize();
	boundsComponent->bounds = Vector2F(static_cast<float>(bitmapSize.width), static_cast<float>(bitmapSize.height));
	this->game->entityManager->AddComponent(entity, boundsComponent);

	auto anchorComponent = std::make_shared<UIAnchorComponent>();
	this->game->entityManager->AddComponent(entity, anchorComponent);

	auto widgetComponent = std::make_shared<UIWidgetComponent>();
	this->game->entityManager->AddComponent(entity, widgetComponent);

	auto visibilityComponent = std::make_shared<VisibilityComponent>();
	this->game->entityManager->AddComponent(entity, visibilityComponent);

	auto tappableComponent = std::make_shared<TappableComponent>();
	this->game->entityManager->AddComponent(entity, tappableComponent);

	return entity;
}

void UIFactory::SetAnchor(Entity entity, VerticalAnchor top, HorizontalAnchor left)
{
	auto anchorComponent = this->game->entityManager->GetComponent<UIAnchorComponent>(entity, UIAnchorComponent::UIAnchorComponentType);
	anchorComponent->top = top;
	anchorComponent->left = left;
}

void UIFactory::SetAnchor(Entity entity, VerticalAnchor top, HorizontalAnchor left, Entity anchorTarget)
{
	auto anchorComponent = this->game->entityManager->GetComponent<UIAnchorComponent>(entity, UIAnchorComponent::UIAnchorComponentType);
	anchorComponent->top = top;
	anchorComponent->left = left;
	anchorComponent->target = anchorTarget;
}

void UIFactory::SetColor(Entity entity,D2D1_COLOR_F color)
{
	auto colorComponent = this->game->entityManager->GetComponent<ColorComponent>(entity, ColorComponent::ColorComponentType);
	colorComponent->color = color;
}

void UIFactory::SetDepth(Entity entity, int depth)
{
	auto depthComponent = this->game->entityManager->GetComponent<DepthComponent>(entity, DepthComponent::DepthComponentType);
	depthComponent->depth = depth;
}

void UIFactory::SetFontSize(Entity entity, float fontSize)
{
	auto fontComponent = this->game->entityManager->GetComponent<FontComponent>(entity, FontComponent::FontComponentType);
	fontComponent->fontSize = fontSize;
}

void UIFactory::SetPanel(Entity entity, Entity panel)
{
	auto widgetComponent = this->game->entityManager->GetComponent<UIWidgetComponent>(entity, UIWidgetComponent::UIWidgetComponentType);
	widgetComponent->panel = panel;
}

void UIFactory::SetText(Entity entity, std::wstring text)
{
	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(entity, TextComponent::TextComponentType);
	textComponent->text = text;
}

void UIFactory::SetVisible(Entity entity, bool visible)
{
	auto visibilityComponent = this->game->entityManager->GetComponent<VisibilityComponent>(entity, VisibilityComponent::VisibilityComponentType);
	visibilityComponent->visible = visible;
}

void UIFactory::SetTappable(Entity entity, bool tappable)
{
	auto tappableComponent = this->game->entityManager->GetComponent<TappableComponent>(entity, TappableComponent::TappableComponentType);
	tappableComponent->tappable = tappable;
}

void UIFactory::FinishUIWidget(Entity entity)
{
	auto entityInitializedEvent = std::make_shared<EntityInitializedEvent>(entity);
	this->game->eventManager->QueueEvent(entityInitializedEvent);
}