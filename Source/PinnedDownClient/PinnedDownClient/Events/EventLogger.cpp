
#include "pch.h"
#include "EventLogger.h"
#include <string>

#include "Events\AppResumingEvent.h"
#include "Events\AppSuspendingEvent.h"
#include "Events\AppWindowChangedEvent.h"
#include "Events\AppWindowSizeChangedEvent.h"
#include "Events\DisplayContentsInvalidatedEvent.h"
#include "Events\DisplayDpiChangedEvent.h"
#include "Events\DisplayOrientationChangedEvent.h"
#include "Events\EntityCreatedEvent.h"
#include "Events\EntityRemovedEvent.h"
#include "Events\GraphicsDeviceLostEvent.h"
#include "Events\GraphicsDeviceRestoredEvent.h"
#include "Events\LoginErrorEvent.h"
#include "Events\LoginSuccessEvent.h"
#include "Events\PointerPressedEvent.h"
#include "Events\RenderTargetChangedEvent.h"

using namespace PinnedDownClient;
using namespace PinnedDownClient::Events;

EventLogger::EventLogger(std::shared_ptr<GameInfrastructure> game)
{
	this->game = game;

	this->game->eventManager->AddListener(this);
}

void EventLogger::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == AppSuspendingEvent::AppSuspendingEventType)
	{
		this->game->logger->Info(L"App suspending.");
	}
	else if (newEvent.GetEventType() == AppResumingEvent::AppResumingEventType)
	{
		this->game->logger->Info(L"App resuming.");
	}
	else if (newEvent.GetEventType() == AppWindowChangedEvent::AppWindowChangedEventType)
	{
		AppWindowChangedEvent appWindowChangedEvent = static_cast<AppWindowChangedEvent&>(newEvent);
		this->game->logger->Info(L"App window changed.");
	}
	else if (newEvent.GetEventType() == AppWindowSizeChangedEvent::AppWindowSizeChangedEventType)
	{
		AppWindowSizeChangedEvent appWindowSizeChangedEvent = static_cast<AppWindowSizeChangedEvent&>(newEvent);
		this->game->logger->Info(L"App window size changed: " + appWindowSizeChangedEvent.size.ToString());
	}
	else if (newEvent.GetEventType() == DisplayContentsInvalidatedEvent::DisplayContentsInvalidatedEventType)
	{
		this->game->logger->Info(L"Display contents invalidated.");
	}
	else if (newEvent.GetEventType() == DisplayDpiChangedEvent::DisplayDpiChangedEventType)
	{
		DisplayDpiChangedEvent displayDpiChangedEvent = static_cast<DisplayDpiChangedEvent&>(newEvent);
		this->game->logger->Info(L"Display DPI changed: " + std::to_wstring(displayDpiChangedEvent.logicalDpi));
	}
	else if (newEvent.GetEventType() == DisplayOrientationChangedEvent::DisplayOrientationChangedEventType)
	{
		DisplayOrientationChangedEvent displayOrientationChangedEvent = static_cast<DisplayOrientationChangedEvent&>(newEvent);
		this->game->logger->Info(L"Display orientation changed: " + std::wstring(displayOrientationChangedEvent.orientation.ToString()->Data()));
	}
	else if (newEvent.GetEventType() == EntityCreatedEvent::EntityCreatedEventType)
	{
		EntityCreatedEvent entityCreatedEvent = static_cast<EntityCreatedEvent&>(newEvent);
		this->game->logger->Info(L"Entity created: " + std::to_wstring(entityCreatedEvent.entityId));
	}
	else if (newEvent.GetEventType() == EntityRemovedEvent::EntityRemovedEventType)
	{
		EntityRemovedEvent entityRemovedEvent = static_cast<EntityRemovedEvent&>(newEvent);
		this->game->logger->Info(L"Entity removed: " + std::to_wstring(entityRemovedEvent.entityId));
	}
	else if (newEvent.GetEventType() == GraphicsDeviceLostEvent::GraphicsDeviceLostEventType)
	{
		this->game->logger->Info(L"Graphics device lost.");
	}
	else if (newEvent.GetEventType() == GraphicsDeviceRestoredEvent::GraphicsDeviceRestoredEventType)
	{
		this->game->logger->Info(L"Graphics device restored.");
	}
	else if (newEvent.GetEventType() == PointerPressedEvent::PointerPressedEventType)
	{
		PointerPressedEvent pointerPressedEvent = static_cast<PointerPressedEvent&>(newEvent);
		this->game->logger->Info(L"Pointer pressed: " + pointerPressedEvent.position.ToString());
	}
	else if (newEvent.GetEventType() == RenderTargetChangedEvent::RenderTargetChangedEventType)
	{
		this->game->logger->Info(L"Render target changed.");
	}
	else if (newEvent.GetEventType() == LoginErrorEvent::LoginErrorEventType)
	{
		LoginErrorEvent loginErrorEvent = static_cast<LoginErrorEvent&>(newEvent);
		this->game->logger->Error(L"Login error: " + loginErrorEvent.errorMessage);
	}
	else if (newEvent.GetEventType() == LoginSuccessEvent::LoginSuccessEventType)
	{
		this->game->logger->Error(L"Login success.");
	}
}