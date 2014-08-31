#include "pch.h"

#include "Game.h"

#include "AssignmentSystem.h"

using namespace PinnedDownCore;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Systems;


AssignmentSystem::AssignmentSystem()
{
}

void AssignmentSystem::InitSystem(PinnedDownCore::Game* game)
{
	GameSystem::InitSystem(game);


}

void AssignmentSystem::OnEvent(Event & newEvent)
{
}

