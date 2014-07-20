//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "pch.h"
#include "PinnedDownGame.h"
#include "Events\AppWindowChangedEvent.h"

#include "Core\SystemManager.h"
#include "Core\EventManager.h"
#include "Core\ResourceManager.h"
#include "Core\EntityManager.h"
#include "Core\FileLogger.h"

#include "Systems\RenderSystem.h"
#include "Systems\InputSystem.h"
#include "Systems\LuaScriptSystem.h"
#include "Systems\SoundSystem.h"

using namespace PinnedDownClient;
using namespace PinnedDownClient::Core;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
PinnedDownGame::PinnedDownGame()
{
	this->gameInfrastructure = std::shared_ptr<GameInfrastructure>(new GameInfrastructure());

	this->gameInfrastructure->logger = std::unique_ptr<FileLogger>(new FileLogger(LogLevel::Debug, L"PinnedDown.log"));
	this->gameInfrastructure->logger->Info(L"Logger initialized.");

	this->gameInfrastructure->resourceManager = std::unique_ptr<ResourceManager>(new Core::ResourceManager());
	this->gameInfrastructure->logger->Info(L"Resource manager initialized.");

	this->gameInfrastructure->eventManager = std::unique_ptr<EventManager>(new EventManager());
	this->gameInfrastructure->logger->Info(L"Event manager initialized.");

	EntityManager* entityManager = new EntityManager(std::shared_ptr<GameInfrastructure>(this->gameInfrastructure));
	this->gameInfrastructure->entityManager = std::unique_ptr<EntityManager>(entityManager);
	this->gameInfrastructure->logger->Info(L"Entity manager initialized.");

	SystemManager* systemManager = new SystemManager(std::shared_ptr<GameInfrastructure>(this->gameInfrastructure));
	this->gameInfrastructure->systemManager = std::unique_ptr<SystemManager>(systemManager);
	this->gameInfrastructure->systemManager->AddSystem(new Systems::RenderSystem());
	this->gameInfrastructure->systemManager->AddSystem(new Systems::InputSystem());
	this->gameInfrastructure->systemManager->AddSystem(new Systems::LuaScriptSystem());
	this->gameInfrastructure->systemManager->AddSystem(new Systems::SoundSystem());

	this->gameInfrastructure->systemManager->InitSystems();
	this->gameInfrastructure->logger->Info(L"System manager initialized.");

	EventLogger* eventLogger = new Events::EventLogger(std::shared_ptr<GameInfrastructure>(this->gameInfrastructure));
	this->eventLogger = std::shared_ptr<Events::EventLogger>();

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

PinnedDownGame::~PinnedDownGame()
{
}

// Updates application state when the window size changes (e.g. device orientation change)
void PinnedDownGame::CreateWindowSizeDependentResources()
{
    // Note to developer: Replace this with the size-dependent initialization of your app's content.
    
    // Input events are dependent on having the correct CoreWindow.
    //m_inputManager->Initialize(CoreWindow::GetForCurrentThread());

    // Only update the virtual controller if it's present.
    //if (m_virtualControllerRenderer != nullptr)
    //{
    //    // Touch regions are dependent on window size and shape.
    //    InitializeTouchRegions();
    //}
}

// Updates the application state once per frame.
void PinnedDownGame::Update()
{
    // Update scene objects.
	timer.Update();

    // Note to developer: Replace these with your app's content update functions.
	this->gameInfrastructure->systemManager->Update(timer);
	this->gameInfrastructure->eventManager->Tick();
	this->gameInfrastructure->entityManager->CleanUpEntities();
	this->gameInfrastructure->eventManager->Tick();

	this->gameInfrastructure->logger->Flush();
		
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
}

// Process all input from the user before updating game state
void PinnedDownGame::ProcessInput(std::vector<PlayerInputData>* playerActions)
{
   
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
	if (timer.GetFrameCount() == 0)
    {
        return false;
    }

	this->gameInfrastructure->systemManager->Render();

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
