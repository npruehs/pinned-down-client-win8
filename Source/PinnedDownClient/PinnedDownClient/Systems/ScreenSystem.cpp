
#include "pch.h"

#include "Event.h"

#include "Events\DisconnectedFromServerEvent.h"
#include "Events\LoginSuccessEvent.h"
#include "Events\ScreenChangedEvent.h"

#include "Systems\ScreenSystem.h"
#include "Systems\Screens\GameScreen.h"
#include "Systems\Screens\LoginScreen.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Systems::UI;
using namespace PinnedDownClient::Systems::Screens;


ScreenSystem::ScreenSystem()
{
}

void ScreenSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);

	this->uiFactory = std::make_shared<UIFactory>(game);

	this->game->eventManager->AddListener(this, ClientIdMappingCreatedEvent::ClientIdMappingCreatedEventType);
	this->game->eventManager->AddListener(this, EntityIdMappingCreatedEvent::EntityIdMappingCreatedEventType);
	this->game->eventManager->AddListener(this, RenderTargetChangedEvent::RenderTargetChangedEventType);
	this->game->eventManager->AddListener(this, LocalizationDataLoadedEvent::LocalizationDataLoadedEventType);
	this->game->eventManager->AddListener(this, LoginSuccessEvent::LoginSuccessEventType);
	this->game->eventManager->AddListener(this, DisconnectedFromServerEvent::DisconnectedFromServerEventType);
}

void ScreenSystem::Update(float dt)
{
	if (this->currentScreen != nullptr)
	{
		this->currentScreen->Update(dt);
	}
}

void ScreenSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(ClientIdMappingCreatedEvent);
	CALL_EVENT_HANDLER(EntityIdMappingCreatedEvent);
	CALL_EVENT_HANDLER(RenderTargetChangedEvent);
	CALL_EVENT_HANDLER(LocalizationDataLoadedEvent);
	CALL_EVENT_HANDLER(LoginSuccessEvent);
	CALL_EVENT_HANDLER(DisconnectedFromServerEvent);
}

EVENT_HANDLER_DEFINITION(ScreenSystem, ClientIdMappingCreatedEvent)
{
	this->clientIdMapping = data.clientIdMapping;
}

EVENT_HANDLER_DEFINITION(ScreenSystem, EntityIdMappingCreatedEvent)
{
	this->entityIdMapping = data.entityIdMapping;
}

EVENT_HANDLER_DEFINITION(ScreenSystem, LoginSuccessEvent)
{
	// Switch to game screen.
	this->SetScreen(std::make_shared<GameScreen>());
}

EVENT_HANDLER_DEFINITION(ScreenSystem, LocalizationDataLoadedEvent)
{
	// Show first screen.
	this->SetScreen(std::make_shared<LoginScreen>());
}

EVENT_HANDLER_DEFINITION(ScreenSystem, RenderTargetChangedEvent)
{
	this->d2dContext = data.d2dContext;
}

EVENT_HANDLER_DEFINITION(ScreenSystem, DisconnectedFromServerEvent)
{
	// Switch to login screen.
	this->SetScreen(std::make_shared<LoginScreen>());
}

void ScreenSystem::SetScreen(std::shared_ptr<Screen> newScreen)
{
	if (this->currentScreen != nullptr)
	{
		this->currentScreen->UnloadUI();
		this->currentScreen->UnloadResources();
		this->currentScreen->DeInitScreen();
	}

	this->currentScreen = newScreen;

	if (this->currentScreen != nullptr)
	{
		this->currentScreen->InitScreen(this->game, this->clientIdMapping, this->entityIdMapping);
		this->currentScreen->LoadResources(this->d2dContext);
		this->currentScreen->LoadUI();

		// Notify listeners.
		auto screenChangedEvent = std::make_shared<ScreenChangedEvent>(this->currentScreen->GetScreenName());
		this->game->eventManager->QueueEvent(screenChangedEvent);
	}
}
