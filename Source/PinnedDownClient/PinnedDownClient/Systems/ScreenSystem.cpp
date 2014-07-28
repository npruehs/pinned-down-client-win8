
#include "pch.h"
#include "Core\Event.h"
#include "Systems\ScreenSystem.h"
#include "Systems\Screens\GameScreen.h"

using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Systems::UI;
using namespace PinnedDownClient::Systems::Screens;

ScreenSystem::ScreenSystem()
{
}

void ScreenSystem::InitSystem(std::shared_ptr<PinnedDownClient::GameInfrastructure> game)
{
	GameSystem::InitSystem(game);

	this->uiFactory = std::make_shared<UIFactory>(game);

	this->game->eventManager->AddListener(std::shared_ptr<IEventListener>(this), RenderTargetChangedEvent::RenderTargetChangedEventType);
}

void ScreenSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == RenderTargetChangedEvent::RenderTargetChangedEventType)
	{
		RenderTargetChangedEvent renderTargetChangedEvent = static_cast<RenderTargetChangedEvent&>(newEvent);
		this->OnRenderTargetChanged(renderTargetChangedEvent);
	}
}

void ScreenSystem::OnRenderTargetChanged(RenderTargetChangedEvent renderTargetChangedEvent)
{
	this->d2dContext = renderTargetChangedEvent.d2dContext;

	// Show first screen.
	this->SetScreen(new GameScreen());
}

void ScreenSystem::SetScreen(IScreen* newScreen)
{
	if (this->currentScreen != nullptr)
	{
		this->currentScreen->UnloadUI();
		this->currentScreen->UnloadResources();
	}

	this->currentScreen = std::shared_ptr<IScreen>(newScreen);

	if (this->currentScreen != nullptr)
	{
		this->currentScreen->InitScreen(this->game);
		this->currentScreen->LoadResources(this->d2dContext);
		this->currentScreen->LoadUI();
	}
}
