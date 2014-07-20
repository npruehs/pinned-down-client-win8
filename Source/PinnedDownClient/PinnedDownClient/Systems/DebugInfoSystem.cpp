#include "pch.h"
#include "Core\Event.h"
#include "Systems\DebugInfoSystem.h"

#include "Components\ColorComponent.h"
#include "Components\FontComponent.h"
#include "Components\ScreenPositionComponent.h"
#include "Components\TextAlignmentComponent.h"
#include "Components\TextComponent.h"
#include "Events\EntityInitializedEvent.h"

using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Components;

DebugInfoSystem::DebugInfoSystem()
{
}

void DebugInfoSystem::InitSystem(std::shared_ptr<PinnedDownClient::GameInfrastructure> game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(std::shared_ptr<IEventListener>(this), PointerMovedEvent::PointerMovedEventType);

	this->CreateEntities();
}

void DebugInfoSystem::CreateEntities()
{
	this->pointerPositionTextEntity = this->game->entityManager->CreateEntity();

	auto colorComponent = std::make_shared<ColorComponent>();
	colorComponent->color = D2D1::ColorF(D2D1::ColorF::White);
	this->game->entityManager->AddComponent(this->pointerPositionTextEntity, colorComponent);

	auto fontComponent = std::make_shared<FontComponent>();
	this->game->entityManager->AddComponent(this->pointerPositionTextEntity, fontComponent);

	auto screenPositionComponent = std::make_shared<ScreenPositionComponent>();
	screenPositionComponent->position = Vector2F(20.0f, 20.0f);
	this->game->entityManager->AddComponent(this->pointerPositionTextEntity, screenPositionComponent);

	auto textComponent = std::make_shared<TextComponent>();
	this->game->entityManager->AddComponent(this->pointerPositionTextEntity, textComponent);

	auto textAlignmentComponent = std::make_shared<TextAlignmentComponent>();
	this->game->entityManager->AddComponent(this->pointerPositionTextEntity, textAlignmentComponent);

	auto entityInitializedEvent = std::shared_ptr<Events::EntityInitializedEvent>(new Events::EntityInitializedEvent(this->pointerPositionTextEntity));
	this->game->eventManager->QueueEvent(entityInitializedEvent);
}

void DebugInfoSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == PointerMovedEvent::PointerMovedEventType)
	{
		PointerMovedEvent pointerMovedEvent = static_cast<PointerMovedEvent&>(newEvent);
		this->OnPointerMoved(pointerMovedEvent);
	}
}

void DebugInfoSystem::OnPointerMoved(PointerMovedEvent pointerMovedEvent)
{
	this->pointerPosition = pointerMovedEvent.position;
	
	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->pointerPositionTextEntity, TextComponent::TextComponentType);
	textComponent->text = L"\nPointer Position: " + this->pointerPosition.ToString();
}
