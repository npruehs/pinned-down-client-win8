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
using namespace PinnedDownClient::Systems::UI;
using namespace PinnedDownClient::Components;

DebugInfoSystem::DebugInfoSystem()
{
}

void DebugInfoSystem::InitSystem(std::shared_ptr<PinnedDownClient::GameInfrastructure> game)
{
	GameSystem::InitSystem(game);

	this->uiFactory = std::make_shared<UIFactory>(this->game);

	this->game->eventManager->AddListener(std::shared_ptr<IEventListener>(this), PointerMovedEvent::PointerMovedEventType);

	this->CreateEntities();
}

void DebugInfoSystem::CreateEntities()
{
	this->pointerPositionTextEntity = this->uiFactory->CreateLabel(L"");
	this->uiFactory->SetAnchor(this->pointerPositionTextEntity,
		VerticalAnchor(VerticalAnchorType::Bottom, -80.0f),
		HorizontalAnchor(HorizontalAnchorType::Left, 20.0f));
	this->uiFactory->FinishUIWidget(this->pointerPositionTextEntity);

	this->fpsTextEntity = this->uiFactory->CreateLabel(L"");
	this->uiFactory->SetAnchor(this->fpsTextEntity,
		VerticalAnchor(VerticalAnchorType::Bottom, -60.0f),
		HorizontalAnchor(HorizontalAnchorType::Left, 20.0f));
	this->uiFactory->FinishUIWidget(this->fpsTextEntity);

	this->versionTextEntity = this->uiFactory->CreateLabel(L"");
	this->uiFactory->SetAnchor(this->versionTextEntity, 
		VerticalAnchor(VerticalAnchorType::Bottom, -40.0f),
		HorizontalAnchor(HorizontalAnchorType::Left, 20.0f));
	this->uiFactory->FinishUIWidget(this->versionTextEntity);

	// Show version number.
	auto version = Package::Current->Id->Version;
	auto textComponent = this->game->entityManager->GetComponent<TextComponent>(this->versionTextEntity, TextComponent::TextComponentType);
	textComponent->text = L"\nVersion " + std::to_wstring(version.Major) + L"." + std::to_wstring(version.Minor) + L"." + std::to_wstring(version.Build) + L"." + std::to_wstring(version.Revision);
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
