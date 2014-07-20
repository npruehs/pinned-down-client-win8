#include "pch.h"
#include "GameInfrastructure.h"
#include "Core\SystemManager.h"

using namespace PinnedDownClient::Core;

SystemManager::SystemManager(std::shared_ptr<PinnedDownClient::GameInfrastructure> game)
{
	this->game = game;
}

void SystemManager::AddSystem(GameSystem* system)
{
	systems.push_back(std::shared_ptr<GameSystem>(system));
}

void SystemManager::InitSystems()
{
	for (unsigned int i = 0; i < systems.size(); i++)
	{
		systems[i]->InitSystem(this->game);
	}
}

void SystemManager::Update(StepTimer const& timer)
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
