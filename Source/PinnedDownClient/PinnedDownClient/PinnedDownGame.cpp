#include "pch.h"
#include "PinnedDownGame.h"
#include "Events\AppWindowChangedEvent.h"

#include "Resources\PinnedDownResourceManager.h"

#include "Systems\AssignmentSystem.h"
#include "Systems\CardIdMappingSystem.h"
#include "Systems\CardLayoutSystem.h"
#include "Systems\CardSelectionHighlightSystem.h"
#include "Systems\ClientIdMappingSystem.h"
#include "Systems\DebugInfoSystem.h"
#include "Systems\EntityIdMappingSystem.h"
#include "Systems\InputSystem.h"
#include "Systems\FightSystem.h"
#include "Systems\LuaScriptSystem.h"
#include "Systems\LocalizationSystem.h"
#include "Systems\NetworkSystem.h"
#include "Systems\CardPlayingSystem.h"
#include "Systems\PlayerSystem.h"
#include "Systems\RenderSystem.h"
#include "Systems\ScreenSystem.h"
#include "Systems\SoundSystem.h"
#include "Systems\SoundEffectSystem.h"
#include "Systems\UIInteractionSystem.h"
#include "Systems\UILayoutSystem.h"

#include "Util\FileLogger.h"
#include "Util\VersionUtils.h"

using namespace Concurrency;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;

using namespace PinnedDownClient;
using namespace PinnedDownClient::Resources;
using namespace PinnedDownClient::Util;

// Loads and initializes application assets when the application is loaded.
PinnedDownGame::PinnedDownGame() :
game(std::make_shared<PinnedDownCore::Game>()),
timer(std::make_unique<Util::StepTimer>())
{
	// Setup game infrastructure.
	this->game->logger = std::unique_ptr<FileLogger>(new FileLogger(LogLevel::Debug, "PinnedDown.log"));
	this->game->logger->LogInfo("Pinned Down Client " + GetPinnedDownClientVersionString());
	this->game->logger->LogInfo("Logger initialized.");

	this->game->resourceManager = std::unique_ptr<ResourceManager>(new PinnedDownResourceManager(this->game.get()));
	this->game->logger->LogInfo("Resource manager initialized.");

	// Init systems.
	this->game->systemManager->AddSystem(std::make_shared<Systems::CardIdMappingSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::EntityIdMappingSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::CardLayoutSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::ClientIdMappingSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::RenderSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::LuaScriptSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::LocalizationSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::SoundSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::DebugInfoSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::UILayoutSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::UIInteractionSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::NetworkSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::ScreenSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::PlayerSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::AssignmentSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::CardSelectionHighlightSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::CardPlayingSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::FightSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::SoundEffectSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::InputSystem>());

	this->game->systemManager->InitSystems();
	this->game->logger->LogInfo("Game initialized.");

	// Setup event logger.
	this->eventLogger = std::make_shared<Events::EventLogger>(this->game);
}

PinnedDownGame::~PinnedDownGame()
{
}

void PinnedDownGame::Update()
{
    // Step timer.
	this->timer->Update();

    // Update game infrastructure.
	this->game->Update((float)this->timer->GetElapsedSeconds());
}

bool PinnedDownGame::Render()
{
    // Don't try to render anything before the first Update.
	if (this->timer->GetFrameCount() == 0)
    {
        return false;
    }

	this->game->systemManager->Render();

    return true;
}
