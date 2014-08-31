#pragma once

#include <map>

#include "EntityManager.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Util
	{
		class EntityIdMapping
		{
		public:
			Entity ClientToServerId(Entity clientEntity);
			Entity ServerToClientId(Entity serverEntity);
			
			void MapEntityId(Entity serverEntity, Entity clientEntity);

		private:
			std::map<Entity, Entity> clientToServerIdMap;
			std::map<Entity, Entity> serverToClientIdMap;
		};
	}
}