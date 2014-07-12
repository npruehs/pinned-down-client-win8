#include "pch.h"
#include "Core\SystemManager.h"

using namespace PinnedDownClient::Core;

SystemManager::SystemManager(std::shared_ptr<EventManager> eventManager, std::shared_ptr<ResourceManager> resourceManager)
{
	this->eventManager = eventManager;
	this->resourceManager = resourceManager;
}

void SystemManager::AddSystem(GameSystem* system)
{
	systems.push_back(std::shared_ptr<GameSystem>(system));
}

void SystemManager::InitSystems()
{
	for (unsigned int i = 0; i < systems.size(); i++)
	{
		systems[i]->InitSystem(this->eventManager, this->resourceManager);
	}
}

void SystemManager::Update(DX::StepTimer const& timer)
{
	for (unsigned int i = 0; i < systems.size(); i++)
	{
		systems[i]->Update(timer);
	}
}

void SystemManager::Render()
{
	for (unsigned int i = 0; i < systems.size(); i++)
	{
		systems[i]->Render();
	}
}
