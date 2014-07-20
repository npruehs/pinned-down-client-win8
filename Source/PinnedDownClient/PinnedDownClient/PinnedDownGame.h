//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include "GameInfrastructure.h"

#include "Helpers\StepTimer.h"
#include "Helpers\InputManager.h"
#include "Helpers\SoundPlayer.h"
#include "Events\EventLogger.h"


// Renders Direct2D and 3D content on the screen.
namespace PinnedDownClient
{
	class PinnedDownGame
    {
    public:
		PinnedDownGame();
		~PinnedDownGame();
        void CreateWindowSizeDependentResources();
        void Update();
        bool Render();

		std::shared_ptr<GameInfrastructure> GetInfrastructure() { return std::shared_ptr<GameInfrastructure>(this->gameInfrastructure); }

    private:
        void InitializeTouchRegions();
        void ProcessInput(std::vector<PlayerInputData>* playerActions);

        // Note to developer: Replace these with your own content rendering.
		std::shared_ptr<GameInfrastructure> gameInfrastructure;

		std::shared_ptr<Events::EventLogger> eventLogger;

        // Input, sound, overlay managers
        std::unique_ptr<InputManager>      m_inputManager;
        std::unique_ptr<SoundPlayer>       m_soundPlayer;

        // Rendering loop timer.
        DX::StepTimer m_timer;
    };
}