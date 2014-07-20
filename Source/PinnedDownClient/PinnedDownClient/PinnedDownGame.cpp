#include "pch.h"
#include "PinnedDownGame.h"
#include "Events\AppWindowChangedEvent.h"

#include "GameInfrastructure.h"

#include "Systems\RenderSystem.h"
#include "Systems\InputSystem.h"
#include "Systems\LuaScriptSystem.h"
#include "Systems\SoundSystem.h"
#include "Systems\DebugInfoSystem.h"

using namespace Concurrency;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;

using namespace PinnedDownClient;
using namespace PinnedDownClient::Core;

// Loads and initializes application assets when the application is loaded.
PinnedDownGame::PinnedDownGame()
{
	// Setup game infrastructure.
	this->gameInfrastructure = std::shared_ptr<GameInfrastructure>(new GameInfrastructure());

	this->gameInfrastructure->timer = std::make_unique<Util::StepTimer>();

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
	this->gameInfrastructure->systemManager->AddSystem(new Systems::DebugInfoSystem());

	this->gameInfrastructure->systemManager->InitSystems();
	this->gameInfrastructure->logger->Info(L"System manager initialized.");

	// Setup event logger.
	EventLogger* eventLogger = new Events::EventLogger(std::shared_ptr<GameInfrastructure>(this->gameInfrastructure));
	this->eventLogger = std::shared_ptr<Events::EventLogger>();
}

PinnedDownGame::~PinnedDownGame()
{
}

void PinnedDownGame::Update()
{
    // Step timer.
	this->gameInfrastructure->timer->Update();

    // Update game infrastructure.
	this->gameInfrastructure->systemManager->Update(*this->gameInfrastructure->timer);
	this->gameInfrastructure->eventManager->Tick();
	this->gameInfrastructure->entityManager->CleanUpEntities();
	this->gameInfrastructure->eventManager->Tick();
	this->gameInfrastructure->logger->Flush();
}

bool PinnedDownGame::Render()
{
    // Don't try to render anything before the first Update.
	if (this->gameInfrastructure->timer->GetFrameCount() == 0)
    {
        return false;
    }

	this->gameInfrastructure->systemManager->Render();

    return true;
}
