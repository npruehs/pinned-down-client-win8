#pragma once

#include <map>

#include "EntityManager.h"
#include "BidirectionalMap.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Util
	{
		class EntityIdMapping
		{
		public:
			EntityIdMapping();

			void Clear();

			Entity ClientToServerId(Entity clientEntity);
			Entity ServerToClientId(Entity serverEntity);
			
			void MapEntityId(Entity serverEntity, Entity clientEntity);

		private:
			std::shared_ptr<BidirectionalMap<Entity, Entity>> serverToClientIdMap;
		};
	}
}