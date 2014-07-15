#include "pch.h"
#include "PinnedDownGame.h"
#include "Core\SystemManager.h"

using namespace PinnedDownClient::Core;

SystemManager::SystemManager(PinnedDownClient::PinnedDownGame* game)
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
