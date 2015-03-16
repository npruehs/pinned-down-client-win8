#include "pch.h"
#include "UIFactory.h"

#include "Source\Features\Globalization\Components\LocalizationComponent.h"
#include "Source\Features\Globalization\Components\TextComponent.h"
#include "Source\Features\Input\Components\TappableComponent.h"
#include "Source\Features\Layout\Components\BoundsComponent.h"
#include "Source\Features\Layout\Components\ScreenPositionComponent.h"
#include "Source\Features\Rendering\Components\ColorComponent.h"
#include "Source\Features\Rendering\Components\DepthComponent.h"
#include "Source\Features\Rendering\Components\FontComponent.h"
#include "Source\Features\Rendering\Components\SpriteComponent.h"
#include "Source\Features\Rendering\Components\TextAlignmentComponent.h"
#include "Source\Features\Rendering\Components\VisibilityComponent.h"
#include "Source\Features\UI\Components\UIPanelComponent.h"
#include "Source\Features\UI\Components\UIWidgetComponent.h"

#include "EntityInitializedEvent.h"

#include "Math\Vector2F.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Features::Globalization;
using namespace PinnedDownClient::Features::Input;
using namespace PinnedDownClient::Features::Layout;
using namespace PinnedDownClient::Features::Rendering;
using namespace PinnedDownClient::Features::UI;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Math;
using namespace PinnedDownClient::Systems::UI;


UIFactory::UIFactory(PinnedDownCore::Game* game)
{
	this->game = game;
}

Entity UIFactory::CreateLabel()
{
	return this->CreateLabel("");
}

Entity UIFactory::CreateLabel(std::string localizationKey)
{
	return this->CreateLabel(localizationKey, 0.0f);
}

Entity UIFactory::CreateLabel(std::string localizationKey, float maxWidth)
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

void UIFactory::SetText(Entity entity, std::string text)
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