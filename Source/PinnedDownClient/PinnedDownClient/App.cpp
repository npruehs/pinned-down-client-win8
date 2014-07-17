#include "pch.h"
#include "App.h"

#include <ppltasks.h>

#include "Events\AppWindowChangedEvent.h"
#include "Events\AppSuspendingEvent.h"
#include "Events\AppResumingEvent.h"
#include "Events\AppWindowSizeChangedEvent.h"
#include "Events\DisplayDpiChangedEvent.h"
#include "Events\DisplayOrientationChangedEvent.h"
#include "Events\DisplayContentsInvalidatedEvent.h"
#include "Events\PointerPressedEvent.h"
#include "Events\PointerMovedEvent.h"
#include "Events\PointerReleasedEvent.h"

using namespace PinnedDownClient;

using namespace concurrency;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

// The main function is only used to initialize our IFrameworkView class.
[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
    auto direct3DApplicationSource = ref new Direct3DApplicationSource();
    CoreApplication::Run(direct3DApplicationSource);
    return 0;
}

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
    return ref new App();
}

App::App()
{
	this->windowClosed = false;
	this->windowVisible = true;
}

// The first method called when the IFrameworkView is being created.
void App::Initialize(CoreApplicationView^ applicationView)
{
    // Register event handlers for app lifecycle. This example includes Activated, so that we
    // can make the CoreWindow active and start rendering on the window.
    applicationView->Activated +=
        ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &App::OnActivated);

    CoreApplication::Suspending +=
        ref new EventHandler<SuspendingEventArgs^>(this, &App::OnSuspending);

    CoreApplication::Resuming +=
        ref new EventHandler<Platform::Object^>(this, &App::OnResuming);

    // At this point we have access to the device. 
    // We can create the device-dependent resources.
    m_deviceResources = std::make_shared<DX::DeviceResources>();
}

// Called when the CoreWindow object is created (or re-created).
void App::SetWindow(CoreWindow^ window)
{
    this->coreWindow = window;
	m_deviceResources->SetWindow(this->coreWindow.Get());

	// Register for window events.
    window->SizeChanged += 
        ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &App::OnWindowSizeChanged);

    window->VisibilityChanged +=
        ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &App::OnVisibilityChanged);

    window->Closed += 
        ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &App::OnWindowClosed);

    // Register for input events.
	window->PointerPressed +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &App::OnPointerPressed);

	window->PointerMoved +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &App::OnPointerMoved);

	window->PointerReleased +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &App::OnPointerReleased);

	// Register for display events.
	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

    currentDisplayInformation->DpiChanged +=
        ref new TypedEventHandler<DisplayInformation^, Object^>(this, &App::OnDpiChanged);

    currentDisplayInformation->OrientationChanged +=
        ref new TypedEventHandler<DisplayInformation^, Object^>(this, &App::OnOrientationChanged);

    DisplayInformation::DisplayContentsInvalidated +=
        ref new TypedEventHandler<DisplayInformation^, Object^>(this, &App::OnDisplayContentsInvalidated);

    // Disable all pointer visual feedback for better performance when touching.
    auto pointerVisualizationSettings = PointerVisualizationSettings::GetForCurrentView();
    pointerVisualizationSettings->IsContactFeedbackEnabled = false; 
    pointerVisualizationSettings->IsBarrelButtonFeedbackEnabled = false;
}

// Initializes scene resources, or loads a previously saved app state.
void App::Load(Platform::String^ entryPoint)
{
	this->game = std::unique_ptr<PinnedDownGame>(new PinnedDownGame(m_deviceResources));

	// Pass window to game.
	auto appWindowChangedEvent = std::shared_ptr<Events::AppWindowChangedEvent>(new Events::AppWindowChangedEvent(this->coreWindow));
	this->game->GetInfrastructure()->eventManager->QueueEvent(appWindowChangedEvent);
}

// This method is called after the window becomes active.
void App::Run()
{
    while (!this->windowClosed)
    {
		if (this->windowVisible)
        {
			this->coreWindow->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			this->game->Update();
			this->game->Render();
        }
        else
        {
			this->coreWindow->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
        }
    }
}

// Required for IFrameworkView.
// Terminate events do not cause Uninitialize to be called. It will be called if your IFrameworkView
// class is torn down while the app is in the foreground.
void App::Uninitialize()
{
}

void App::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
    // Run() won't start until the CoreWindow is activated.
	this->coreWindow->Activate();
}

void App::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
    // Save app state asynchronously after requesting a deferral. Holding a deferral
    // indicates that the application is busy performing suspending operations. Be
    // aware that a deferral may not be held indefinitely. After about five seconds,
    // the app will be forced to exit.
    SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

    create_task([this, deferral]()
    {
		// Allow subsystems to save their state.
		auto appSuspendingEvent = std::shared_ptr<Events::AppSuspendingEvent>(new Events::AppSuspendingEvent());
		this->game->GetInfrastructure()->eventManager->RaiseEvent(appSuspendingEvent);

        deferral->Complete();
    });
}

void App::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
    // Restore any data or state that was unloaded on suspend. By default, data
    // and state are persisted when resuming from suspend. Note that this event
    // does not occur if the app was previously terminated.
	auto appResumingEvent = std::shared_ptr<Events::AppResumingEvent>(new Events::AppResumingEvent());
	this->game->GetInfrastructure()->eventManager->RaiseEvent(appResumingEvent);
}

void App::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	this->coreWindow = sender;
	m_deviceResources->SetWindow(this->coreWindow.Get());

	float width = sender->Bounds.Width;
	float height = sender->Bounds.Height;

	m_deviceResources->SetLogicalSize(Size(width, height));
	this->game->CreateWindowSizeDependentResources();

	// Notify subsystems.
	auto appWindowSizeChangedEvent = std::shared_ptr<Events::AppWindowSizeChangedEvent>(new Events::AppWindowSizeChangedEvent(width, height));
	this->game->GetInfrastructure()->eventManager->QueueEvent(appWindowSizeChangedEvent);
}

void App::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	this->windowVisible = args->Visible;
}

void App::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	this->windowClosed = true;
}

void App::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
    m_deviceResources->SetDpi(sender->LogicalDpi);
	this->game->CreateWindowSizeDependentResources();

	float dpi = sender->LogicalDpi;

	// Notify subsystems.
	auto displayDpiChangedEvent = std::shared_ptr<Events::DisplayDpiChangedEvent>(new Events::DisplayDpiChangedEvent(dpi));
	this->game->GetInfrastructure()->eventManager->QueueEvent(displayDpiChangedEvent);
}

void App::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
    m_deviceResources->SetCurrentOrientation(sender->CurrentOrientation);
	this->game->CreateWindowSizeDependentResources();

	Windows::Graphics::Display::DisplayOrientations orientation = sender->CurrentOrientation;

	// Notify subsystems.
	auto displayOrientationChangedEvent = std::shared_ptr<Events::DisplayOrientationChangedEvent>(new Events::DisplayOrientationChangedEvent(orientation));
	this->game->GetInfrastructure()->eventManager->QueueEvent(displayOrientationChangedEvent);
}

void App::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
    m_deviceResources->ValidateDevice();

	// Notify subsystems.
	auto displayContentsInvalidatedEvent = std::shared_ptr<Events::DisplayContentsInvalidatedEvent>(new Events::DisplayContentsInvalidatedEvent());
	this->game->GetInfrastructure()->eventManager->QueueEvent(displayContentsInvalidatedEvent);
}

void App::OnPointerPressed(_In_ Windows::UI::Core::CoreWindow^ sender, _In_ Windows::UI::Core::PointerEventArgs^ args)
{	
	auto point = args->CurrentPoint;

	// Notify subsystems.
	auto pointerPressedEvent = std::shared_ptr<Events::PointerPressedEvent>(new Events::PointerPressedEvent(point->PointerId, point->Position.X, point->Position.Y));
	this->game->GetInfrastructure()->eventManager->QueueEvent(pointerPressedEvent);
}

void App::OnPointerMoved(_In_ Windows::UI::Core::CoreWindow^ sender, _In_ Windows::UI::Core::PointerEventArgs^ args)
{
	auto point = args->CurrentPoint;

	// Notify subsystems.
	auto pointerMovedEvent = std::shared_ptr<Events::PointerMovedEvent>(new Events::PointerMovedEvent(point->PointerId, point->Position.X, point->Position.Y));
	this->game->GetInfrastructure()->eventManager->QueueEvent(pointerMovedEvent);
}

void App::OnPointerReleased(_In_ Windows::UI::Core::CoreWindow^ sender, _In_ Windows::UI::Core::PointerEventArgs^ args)
{
	auto point = args->CurrentPoint;

	// Notify subsystems.
	auto pointerReleasedEvent = std::shared_ptr<Events::PointerReleasedEvent>(new Events::PointerReleasedEvent(point->PointerId, point->Position.X, point->Position.Y));
	this->game->GetInfrastructure()->eventManager->QueueEvent(pointerReleasedEvent);
}