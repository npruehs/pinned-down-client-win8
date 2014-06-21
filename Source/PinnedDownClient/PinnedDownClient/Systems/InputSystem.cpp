
#include "pch.h"
#include "Core\Event.h"
#include "Core\EventManager.h"
#include "Systems\InputSystem.h"

using namespace PinnedDownClient::Systems;

InputSystem::InputSystem()
{
}

void InputSystem::InitSystem(std::shared_ptr<EventManager> eventManager)
{
	ISystem::InitSystem(eventManager);

	eventManager->AddListener(std::shared_ptr<IEventListener>(this), Util::HashedString("AppWindowChanged"));
	eventManager->AddListener(std::shared_ptr<IEventListener>(this), Util::HashedString("AppWindowSizeChanged"));
}

void InputSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == Util::HashedString("AppWindowChanged"))
	{
		Events::AppWindowChangedEvent appWindowChangedEvent = static_cast<Events::AppWindowChangedEvent&>(newEvent);
		this->OnAppWindowChanged(appWindowChangedEvent);
	}
	else if (newEvent.GetEventType() == Util::HashedString("AppWindowSizeChanged"))
	{
		Events::AppWindowSizeChangedEvent appWindowSizeChangedEvent = static_cast<Events::AppWindowSizeChangedEvent&>(newEvent);
		this->OnAppWindowSizeChanged(appWindowSizeChangedEvent);
	}
}

void InputSystem::OnAppWindowChanged(PinnedDownClient::Events::AppWindowChangedEvent appWindowChangedEvent)
{
}

void InputSystem::OnAppWindowSizeChanged(PinnedDownClient::Events::AppWindowSizeChangedEvent appWindowSizeChangedEvent)
{
}

void InputSystem::Update(DX::StepTimer const& timer)
{
}
