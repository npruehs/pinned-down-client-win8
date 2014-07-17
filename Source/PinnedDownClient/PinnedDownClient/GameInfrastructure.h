#pragma once

#include "Core\SystemManager.h"
#include "Core\EventManager.h"
#include "Core\ResourceManager.h"
#include "Core\EntityManager.h"
#include "Core\FileLogger.h"

namespace PinnedDownClient
{
	class GameInfrastructure
	{
	public:
		GameInfrastructure() {}

		std::unique_ptr<Core::EventManager> eventManager;
		std::unique_ptr<Core::SystemManager> systemManager;
		std::unique_ptr<Core::ResourceManager> resourceManager;
		std::unique_ptr<Core::EntityManager> entityManager;
		std::unique_ptr<Core::FileLogger> logger;
	};
}