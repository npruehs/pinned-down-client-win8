#include "pch.h"
#include "Core\Event.h"
#include "Systems\DebugInfoSystem.h"

#include "Components\ColorComponent.h"
#include "Components\FontComponent.h"
#include "Components\ScreenPositionComponent.h"
#include "Components\TextAlignmentComponent.h"
#include "Components\TextComponent.h"
#include "Events\EntityInitializedEvent.h"

using namespace Windows::ApplicationModel;

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
	this->pointerPositionTextEntity = this->CreateTextEntity(Vector2F(20.0f, 20.0f));
	this->fpsTextEntity = this->CreateTextEntity(Vector2F(20.0f, 40.0f));
	this->versionTextEntity = this->CreateTextEntity(Vector2F(20.0f, 60.0f));

	// Show version number.
	auto version = Package::Current->Id->Version;
	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->versionTextEntity, TextComponent::TextComponentType);
	textComponent->text = L"\nVersion " + std::to_wstring(version.Major) + L"." + std::to_wstring(version.Minor) + L"." + std::to_wstring(version.Build) + L"." + std::to_wstring(version.Revision);
}

int DebugInfoSystem::CreateTextEntity(Vector2F screenPosition)
{
	int entityId = this->game->entityManager->CreateEntity();

	auto colorComponent = std::make_shared<ColorComponent>();
	colorComponent->color = D2D1::ColorF(D2D1::ColorF::White);
	this->game->entityManager->AddComponent(entityId, colorComponent);

	auto fontComponent = std::make_shared<FontComponent>();
	this->game->entityManager->AddComponent(entityId, fontComponent);

	auto screenPositionComponent = std::make_shared<ScreenPositionComponent>();
	screenPositionComponent->position = screenPosition;
	this->game->entityManager->AddComponent(entityId, screenPositionComponent);

	auto textComponent = std::make_shared<TextComponent>();
	this->game->entityManager->AddComponent(entityId, textComponent);

	auto textAlignmentComponent = std::make_shared<TextAlignmentComponent>();
	this->game->entityManager->AddComponent(entityId, textAlignmentComponent);

	auto entityInitializedEvent = std::shared_ptr<Events::EntityInitializedEvent>(new Events::EntityInitializedEvent(entityId));
	this->game->eventManager->QueueEvent(entityInitializedEvent);

	return entityId;
}

void DebugInfoSystem::Update(StepTimer const& timer)
{
	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->fpsTextEntity, TextComponent::TextComponentType);
	textComponent->text = L"\nFPS: " + std::to_wstring(timer.GetFramesPerSecond());
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
