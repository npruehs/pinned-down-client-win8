#include "pch.h"
#include "Event.h"
#include "Systems\DebugInfoSystem.h"

#include "Components\ColorComponent.h"
#include "Components\FontComponent.h"
#include "Components\PositionComponent.h"
#include "Components\ScreenPositionComponent.h"
#include "Components\TextAlignmentComponent.h"
#include "Components\TextComponent.h"

#include "EntityInitializedEvent.h"

#include "..\Util\VersionUtils.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Systems::UI;
using namespace PinnedDownClient::Components;
using namespace PinnedDownClient::Util;

DebugInfoSystem::DebugInfoSystem()
{
}

void DebugInfoSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->uiFactory = std::make_shared<UIFactory>(this->game);
	this->timer = std::make_shared<StepTimer>();

	this->game->eventManager->AddListener(this, CardCameraCreatedEvent::CardCameraCreatedEventType);
	this->game->eventManager->AddListener(this, PointerMovedEvent::PointerMovedEventType);

	this->CreateEntities();
}

void DebugInfoSystem::CreateEntities()
{
	this->cameraPositionTextEntity = this->uiFactory->CreateLabel("");
	this->uiFactory->SetAnchor(this->cameraPositionTextEntity,
		VerticalAnchor(VerticalAnchorType::Bottom, cameraPositionTextPosition.y),
		HorizontalAnchor(HorizontalAnchorType::Left, cameraPositionTextPosition.x));
	this->uiFactory->FinishUIWidget(this->cameraPositionTextEntity);

	this->pointerPositionTextEntity = this->uiFactory->CreateLabel("");
	this->uiFactory->SetAnchor(this->pointerPositionTextEntity,
		VerticalAnchor(VerticalAnchorType::Bottom, pointerPositionTextPosition.y),
		HorizontalAnchor(HorizontalAnchorType::Left, pointerPositionTextPosition.x));
	this->uiFactory->FinishUIWidget(this->pointerPositionTextEntity);

	this->fpsTextEntity = this->uiFactory->CreateLabel("");
	this->uiFactory->SetAnchor(this->fpsTextEntity,
		VerticalAnchor(VerticalAnchorType::Bottom, fpsTextPosition.y),
		HorizontalAnchor(HorizontalAnchorType::Left, fpsTextPosition.x));
	this->uiFactory->FinishUIWidget(this->fpsTextEntity);

	this->versionTextEntity = this->uiFactory->CreateLabel("");
	this->uiFactory->SetAnchor(this->versionTextEntity, 
		VerticalAnchor(VerticalAnchorType::Bottom, versionTextPosition.y),
		HorizontalAnchor(HorizontalAnchorType::Left, versionTextPosition.x));
	this->uiFactory->FinishUIWidget(this->versionTextEntity);

	// Show version number.
	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->versionTextEntity, TextComponent::TextComponentType);
	textComponent->text = "\n" + GetPinnedDownClientVersionString();
}

void DebugInfoSystem::Update(float dt)
{
	this->timer->Update();

	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->fpsTextEntity, TextComponent::TextComponentType);
	textComponent->text = "\nFPS: " + std::to_string(this->timer->GetFramesPerSecond());
}

void DebugInfoSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(CardCameraCreatedEvent);
	CALL_EVENT_HANDLER(PointerMovedEvent);
}

EVENT_HANDLER_DEFINITION(DebugInfoSystem, CardCameraCreatedEvent)
{
	this->cardCamera = data.entity;
}

EVENT_HANDLER_DEFINITION(DebugInfoSystem, PointerMovedEvent)
{
	this->pointerPosition = data.position;
	
	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->pointerPositionTextEntity, TextComponent::TextComponentType);
	textComponent->text = "\nPointer Position: " + this->pointerPosition.ToString();
}

EVENT_HANDLER_DEFINITION(DebugInfoSystem, PositionChangedEvent)
{
	if (this->cardCamera == INVALID_ENTITY_ID || data.entity != this->cardCamera)
	{
		return;
	}

	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->cameraPositionTextEntity, TextComponent::TextComponentType);
	textComponent->text = "\nCamera Position: " + data.position.ToString();
}