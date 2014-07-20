#pragma once

#include "GameInfrastructure.h"

#include "Util\StepTimer.h"
#include "Events\EventLogger.h"

namespace PinnedDownClient
{
	class PinnedDownGame
    {
    public:
		PinnedDownGame();
		~PinnedDownGame();

		// Updates the application state once per frame.
        void Update();

		// Renders the current frame according to the current application state.
		// Returns true if the frame was rendered and is ready to be displayed.
		bool Render();

		std::shared_ptr<GameInfrastructure> GetInfrastructure() { return std::shared_ptr<GameInfrastructure>(this->gameInfrastructure); }

    private:
        // Note to developer: Replace these with your own content rendering.
		std::shared_ptr<GameInfrastructure> gameInfrastructure;

		// Logger tracking all game events.
		std::shared_ptr<Events::EventLogger> eventLogger;
    };
}