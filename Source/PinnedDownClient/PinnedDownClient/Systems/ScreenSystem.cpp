
#include "pch.h"

#include "Event.h"

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

	this->game->eventManager->AddListener(this, RenderTargetChangedEvent::RenderTargetChangedEventType);
	this->game->eventManager->AddListener(this, LoginSuccessEvent::LoginSuccessEventType);
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
	if (newEvent.GetEventType() == RenderTargetChangedEvent::RenderTargetChangedEventType)
	{
		auto renderTargetChangedEvent = static_cast<RenderTargetChangedEvent&>(newEvent);
		this->OnRenderTargetChanged(renderTargetChangedEvent);
	}
	else if (newEvent.GetEventType() == LoginSuccessEvent::LoginSuccessEventType)
	{
		auto loginSuccessEvent = static_cast<LoginSuccessEvent&>(newEvent);
		this->OnLoginSuccess();
	}
}

void ScreenSystem::OnLoginSuccess()
{
	// Switch to game screen.
	this->SetScreen(std::make_shared<GameScreen>());
}

void ScreenSystem::OnRenderTargetChanged(RenderTargetChangedEvent renderTargetChangedEvent)
{
	this->d2dContext = renderTargetChangedEvent.d2dContext;

	// Show first screen.
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
		this->currentScreen->InitScreen(this->game);
		this->currentScreen->LoadResources(this->d2dContext);
		this->currentScreen->LoadUI();

		// Notify listeners.
		auto screenChangedEvent = std::make_shared<ScreenChangedEvent>(this->currentScreen->GetScreenName());
		this->game->eventManager->QueueEvent(screenChangedEvent);
	}
}
