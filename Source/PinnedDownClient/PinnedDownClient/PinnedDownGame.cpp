﻿#include "pch.h"
#include "PinnedDownGame.h"
#include "Events\AppWindowChangedEvent.h"

#include "Core\PinnedDownResourceManager.h"
#include "Core\FileLogger.h"

#include "Systems\RenderSystem.h"
#include "Systems\LuaScriptSystem.h"
#include "Systems\SoundSystem.h"
#include "Systems\DebugInfoSystem.h"
#include "Systems\UIInteractionSystem.h"
#include "Systems\UILayoutSystem.h"
#include "Systems\NetworkSystem.h"
#include "Systems\ScreenSystem.h"

using namespace Concurrency;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;

using namespace PinnedDownClient;
using namespace PinnedDownClient::Core;

// Loads and initializes application assets when the application is loaded.
PinnedDownGame::PinnedDownGame()
{
	// Setup game infrastructure.
	this->game = std::make_shared<PinnedDownCore::Game>();
	this->timer = std::make_unique<Util::StepTimer>();

	this->game->logger = std::unique_ptr<FileLogger>(new FileLogger(LogLevel::Debug, L"PinnedDown.log"));
	this->game->logger->Info(L"Logger initialized.");

	this->game->resourceManager = std::unique_ptr<ResourceManager>(new Core::PinnedDownResourceManager());
	this->game->logger->Info(L"Resource manager initialized.");

	// Init systems.
	this->game->systemManager->AddSystem(new Systems::RenderSystem());
	this->game->systemManager->AddSystem(new Systems::LuaScriptSystem());
	this->game->systemManager->AddSystem(new Systems::SoundSystem());
	this->game->systemManager->AddSystem(new Systems::DebugInfoSystem());
	this->game->systemManager->AddSystem(new Systems::UILayoutSystem());
	this->game->systemManager->AddSystem(new Systems::UIInteractionSystem());
	this->game->systemManager->AddSystem(new Systems::NetworkSystem());
	this->game->systemManager->AddSystem(new Systems::ScreenSystem());

	this->game->systemManager->InitSystems();
	this->game->logger->Info(L"Game initialized.");

	// Setup event logger.
	EventLogger* eventLogger = new Events::EventLogger(this->game);
	this->eventLogger = std::shared_ptr<Events::EventLogger>();
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
