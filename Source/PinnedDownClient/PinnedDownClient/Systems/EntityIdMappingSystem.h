#pragma once

#include "IEventListener.h"
#include "GameSystem.h"

#include "Util\EntityIdMapping.h"


using namespace PinnedDownClient::Util;


namespace PinnedDownClient
{
	namespace Systems
	{
		class EntityIdMappingSystem : public GameSystem, public IEventListener
		{
		public:
			EntityIdMappingSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::shared_ptr<EntityIdMapping> entityIdMapping;

			void OnEvent(Event & event);
		};
	}
}
