
#include "pch.h"
#include "Core\Event.h"
#include "Systems\InputSystem.h"

using namespace PinnedDownClient::Systems;

InputSystem::InputSystem()
{
}

void InputSystem::InitSystem(PinnedDownClient::PinnedDownGame* game)
{
	GameSystem::InitSystem(game);

	this->game->GetEventManager()->AddListener(std::shared_ptr<IEventListener>(this), AppWindowChangedEvent::AppWindowChangedEventType);
	this->game->GetEventManager()->AddListener(std::shared_ptr<IEventListener>(this), AppWindowSizeChangedEvent::AppWindowSizeChangedEventType);
	this->game->GetEventManager()->AddListener(std::shared_ptr<IEventListener>(this), DisplayDpiChangedEvent::DisplayDpiChangedEventType);
	this->game->GetEventManager()->AddListener(std::shared_ptr<IEventListener>(this), DisplayOrientationChangedEvent::DisplayOrientationChangedEventType);
}

void InputSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == AppWindowChangedEvent::AppWindowChangedEventType)
	{
		AppWindowChangedEvent appWindowChangedEvent = static_cast<AppWindowChangedEvent&>(newEvent);
		this->OnAppWindowChanged(appWindowChangedEvent);
	}
	else if (newEvent.GetEventType() == AppWindowSizeChangedEvent::AppWindowSizeChangedEventType)
	{
		AppWindowSizeChangedEvent appWindowSizeChangedEvent = static_cast<AppWindowSizeChangedEvent&>(newEvent);
		this->OnAppWindowSizeChanged(appWindowSizeChangedEvent);
	}
	else if (newEvent.GetEventType() == DisplayDpiChangedEvent::DisplayDpiChangedEventType)
	{
		DisplayDpiChangedEvent displayDpiChangedEvent = static_cast<DisplayDpiChangedEvent&>(newEvent);
		this->OnDisplayDpiChanged(displayDpiChangedEvent);
	}
	else if (newEvent.GetEventType() == DisplayOrientationChangedEvent::DisplayOrientationChangedEventType)
	{
		DisplayOrientationChangedEvent displayOrientationChangedEvent = static_cast<DisplayOrientationChangedEvent&>(newEvent);
		this->OnDisplayOrientationChanged(displayOrientationChangedEvent);
	}
}

void InputSystem::OnAppWindowChanged(AppWindowChangedEvent appWindowChangedEvent)
{
}

void InputSystem::OnAppWindowSizeChanged(AppWindowSizeChangedEvent appWindowSizeChangedEvent)
{
}

void InputSystem::OnDisplayDpiChanged(DisplayDpiChangedEvent displayDpiChangedEvent)
{
}

void InputSystem::OnDisplayOrientationChanged(DisplayOrientationChangedEvent displayOrientationChangedEvent)
{
}

