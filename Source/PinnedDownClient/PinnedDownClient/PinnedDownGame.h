#pragma once

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

		std::shared_ptr<PinnedDownCore::Game> GetGame() { return std::shared_ptr<PinnedDownCore::Game>(this->game); }

    private:
        // Note to developer: Replace these with your own content rendering.
		std::shared_ptr<PinnedDownCore::Game> game;

		std::shared_ptr<Util::StepTimer> timer;

		// Logger tracking all game events.
		std::shared_ptr<Events::EventLogger> eventLogger;
    };
}