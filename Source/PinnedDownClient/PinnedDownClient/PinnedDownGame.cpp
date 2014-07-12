//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "pch.h"
#include "PinnedDownGame.h"
#include "Helpers\DirectXHelper.h"

#include "Core\SystemManager.h"
#include "Systems\RenderSystem.h"
#include "Systems\InputSystem.h"
#include "Events\AppWindowChangedEvent.h"

using namespace PinnedDownClient;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
PinnedDownGame::PinnedDownGame(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
    m_deviceResources(deviceResources)
{
    // Register to be notified if the Device is lost or recreated.
    m_deviceResources->RegisterDeviceNotify(this);

	this->resourceManager = std::shared_ptr<Core::ResourceManager>(new Core::ResourceManager());

	this->eventManager = std::shared_ptr<Core::EventManager>(new Core::EventManager());

	this->systemManager = std::shared_ptr<Core::SystemManager>(new Core::SystemManager(this->eventManager, this->resourceManager));
	this->systemManager->AddSystem(new Systems::RenderSystem());
	this->systemManager->AddSystem(new Systems::InputSystem());

	this->systemManager->InitSystems();

    // Note to developer: Replace this with your app's content initialization.
    //m_debugTextRenderer = std::shared_ptr<SampleDebugTextRenderer>(new SampleDebugTextRenderer(m_deviceResources));

    //// Note to developer: Use these to get input data, play audio, and draw HUDs and menus.
    //m_inputManager   = std::unique_ptr<InputManager>(new InputManager());
    //m_soundPlayer    = std::unique_ptr<SoundPlayer>(new SoundPlayer());
    //m_overlayManager = std::unique_ptr<OverlayManager>(new OverlayManager(m_deviceResources));

    //// This vector will be sent to the overlay manager.
    //std::vector<std::shared_ptr<Overlay>> overlays;
    //overlays.push_back(m_debugTextRenderer);

    //// Check whether the device is touch capable before setting up the virtual controller.
    //TouchCapabilities^ pTouchCapabilities = ref new TouchCapabilities();
    //if (pTouchCapabilities->TouchPresent != 0)
    //{
    //    // Create and add virtual controller overlay.
    //    m_virtualControllerRenderer = std::shared_ptr<SampleVirtualControllerRenderer>(new SampleVirtualControllerRenderer(m_deviceResources));
    //    overlays.push_back(m_virtualControllerRenderer);

    //    // Set up touch regions.
    //    InitializeTouchRegions();
    //}

    //// Send the final list of overlays to the overlay manager.
    //m_overlayManager->SetOverlays(overlays);

    //// Note to developer: Apply input to your game.
    //// This template supports all control types by default.
    //m_inputManager->SetFilter(INPUT_DEVICE_ALL);
    //m_inputManager->Initialize(CoreWindow::GetForCurrentThread());

    // Note to developer: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */
}

void PinnedDownGame::InitializeTouchRegions()
{
    // Here we set up the touch control regions.
    Windows::Foundation::Size logicalSize = m_deviceResources->GetLogicalSize();
    XMFLOAT2 screenTopLeft = XMFLOAT2(0, 0);
    XMFLOAT2 screenTopRight = XMFLOAT2(logicalSize.Width, 0);
    XMFLOAT2 screenBottomLeft = XMFLOAT2(0, logicalSize.Height);
    XMFLOAT2 screenBottomRight = XMFLOAT2(logicalSize.Width, logicalSize.Height);
    float width = screenTopRight.x - screenTopLeft.x;
    float height = screenBottomLeft.y - screenTopLeft.y;

    // Clear previous touch regions.
    m_virtualControllerRenderer->ClearTouchControlRegions();
    m_inputManager->ClearTouchRegions();

    // The following scoped code region sets up the analog stick.
    {
        const float stickRegionPercent = 0.75f;
        float stickRegionWidth = (width * stickRegionPercent);
        XMFLOAT2 touchRegionBoundary = XMFLOAT2(screenTopLeft.x + stickRegionWidth, screenBottomRight.y);

        TouchControlRegion touchControlRegionStick(
            screenTopLeft,
            touchRegionBoundary,
            TOUCH_CONTROL_REGION_ANALOG_STICK,
            PLAYER_ACTION_TYPES::INPUT_MOVE,
            PLAYER_ID::PLAYER_ID_ONE
            );

        DWORD errorCode = m_inputManager->SetDefinedTouchRegion(&touchControlRegionStick, m_touchRegionIDs[0]);

        if (!errorCode)
        {
            m_virtualControllerRenderer->AddTouchControlRegion(touchControlRegionStick);
        }
    }

    // The following scoped code region sets up the buttons.
    {
        const float buttonRegionPercent = 0.2f;
        const float buttonWidthHeight   = 80.f;
        const float buttonDesiredOffset = 300.f;

        // Control the max location to prevent overlap
        float buttonWidthOffset = (width * buttonRegionPercent) - buttonWidthHeight;
        buttonWidthOffset = buttonWidthOffset < buttonDesiredOffset ? buttonWidthOffset : buttonDesiredOffset;

        // Set up button A
        XMFLOAT2 location1 = { width - (buttonWidthOffset + 1.f * buttonWidthHeight), height - buttonDesiredOffset };
        TouchControlRegion touchControlRegionButtonA(
            XMFLOAT2(location1.x, location1.y),
            XMFLOAT2(location1.x + buttonWidthHeight, location1.y + buttonWidthHeight),
            TOUCH_CONTROL_REGION_TYPES::TOUCH_CONTROL_REGION_BUTTON,
            PLAYER_ACTION_TYPES::INPUT_FIRE_DOWN,
            PLAYER_ID::PLAYER_ID_ONE
            );

        DWORD errorCode = m_inputManager->SetDefinedTouchRegion(&touchControlRegionButtonA, m_touchRegionIDs[1]);
        if (!errorCode)
        {
            m_virtualControllerRenderer->AddTouchControlRegion(touchControlRegionButtonA);
        }

        // Set up button B
        XMFLOAT2 location2 = { width - buttonWidthOffset, height - buttonDesiredOffset - (1.f * buttonWidthHeight) };
        TouchControlRegion touchControlRegionButtonB(
            XMFLOAT2(location2.x, location2.y),
            XMFLOAT2(location2.x + buttonWidthHeight, location2.y + buttonWidthHeight),
            TOUCH_CONTROL_REGION_TYPES::TOUCH_CONTROL_REGION_BUTTON,
            PLAYER_ACTION_TYPES::INPUT_JUMP_DOWN,
            PLAYER_ID::PLAYER_ID_ONE
            );

        errorCode = m_inputManager->SetDefinedTouchRegion(&touchControlRegionButtonB, m_touchRegionIDs[2]);
        if (!errorCode)
        {
            m_virtualControllerRenderer->AddTouchControlRegion(touchControlRegionButtonB);
        }
    }
}


PinnedDownGame::~PinnedDownGame()
{
    // Deregister device notification
    m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void PinnedDownGame::CreateWindowSizeDependentResources()
{
    // Note to developer: Replace this with the size-dependent initialization of your app's content.
    
    // Input events are dependent on having the correct CoreWindow.
    m_inputManager->Initialize(CoreWindow::GetForCurrentThread());

    // Only update the virtual controller if it's present.
    if (m_virtualControllerRenderer != nullptr)
    {
        // Touch regions are dependent on window size and shape.
        InitializeTouchRegions();
    }
}

// Updates the application state once per frame.
void PinnedDownGame::Update()
{
    // Update scene objects.
    m_timer.Tick([&]()
    {
        // Note to developer: Replace these with your app's content update functions.
		eventManager->Tick();
		systemManager->Update(m_timer);
		
		//m_overlayManager->Update(m_timer);
  //      m_inputManager->Update(m_timer);

  //      std::vector<PlayerInputData> playerActions;
  //      ProcessInput(&playerActions);

  //      m_debugTextRenderer->Update(&playerActions, m_playersConnected);

  //      // Only update the virtual controller if it's present.
  //      if (m_virtualControllerRenderer != nullptr)
  //      {
  //          m_virtualControllerRenderer->Update(&playerActions);
  //      }
    });
}

// Process all input from the user before updating game state
void PinnedDownGame::ProcessInput(std::vector<PlayerInputData>* playerActions)
{
    m_playersConnected = m_inputManager->GetPlayersConnected();

    *playerActions = m_inputManager->GetPlayersActions();

    for (unsigned int j = 0; j < playerActions->size(); j++)
    {
        PlayerInputData playerAction = (*playerActions)[j];

        if (playerAction.ID == 0)

        switch (playerAction.PlayerAction)
        {
        case PLAYER_ACTION_TYPES::INPUT_FIRE_PRESSED:
            m_soundPlayer->PlaySound(std::wstring(L"assets/chord.wav"));
            break;

        case PLAYER_ACTION_TYPES::INPUT_START:
            m_soundPlayer->PlayMusic(std::wstring(L"assets/becky.wma"));
            break;

        default:
            break;
        }
    }
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool PinnedDownGame::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return false;
    }

	systemManager->Render();

    //auto context = m_deviceResources->GetD3DDeviceContext();

    //// Reset the viewport to target the whole screen.
    //auto viewport = m_deviceResources->GetScreenViewport();
    //context->RSSetViewports(1, &viewport);

    //// Reset render targets to the screen.
    //ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
    //context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

    //// Clear the back buffer and depth stencil view.
    //context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
    //context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    //// Render the scene objects.
    //// Note to developer: Replace this with your app's content rendering functions.
    //m_overlayManager->Render();

    return true;
}

// Notifies renderers that device resources need to be released.
void PinnedDownGame::OnDeviceLost()
{
    m_overlayManager->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void PinnedDownGame::OnDeviceRestored()
{
    m_overlayManager->CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
}
