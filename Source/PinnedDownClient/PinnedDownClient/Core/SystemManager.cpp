#include "pch.h"
#include "Core\SystemManager.h"

using namespace PinnedDownClient::Core;

SystemManager::SystemManager()
{
}

void SystemManager::AddSystem(ISystem* system)
{
	systems.push_back(std::shared_ptr<ISystem>(system));
}

void SystemManager::InitSystems()
{
	for (unsigned int i = 0; i < systems.size(); i++)
	{
		systems[i]->InitSystem();
	}
}

void SystemManager::Update(DX::StepTimer const& timer)
{
	for (unsigned int i = 0; i < systems.size(); i++)
	{
		systems[i]->Update(timer);
	}
}
