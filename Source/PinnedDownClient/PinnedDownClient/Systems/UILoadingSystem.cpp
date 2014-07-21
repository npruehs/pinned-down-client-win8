
#include "pch.h"
#include "Core\Event.h"
#include "Systems\UILoadingSystem.h"

using namespace PinnedDownClient::Systems;

UILoadingSystem::UILoadingSystem()
{
}

void UILoadingSystem::InitSystem(std::shared_ptr<PinnedDownClient::GameInfrastructure> game)
{
	GameSystem::InitSystem(game);

	this->LoadUI();
}

void UILoadingSystem::OnEvent(Event & newEvent)
{
}

void UILoadingSystem::LoadUI()
{
}