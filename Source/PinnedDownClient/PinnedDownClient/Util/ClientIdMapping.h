#pragma once

#include <map>

#include "BidirectionalMap.h"
#include "EntityManager.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Util
	{
		class ClientIdMapping
		{
		public:
			ClientIdMapping();

			void Clear();

			bool IsLocalPlayer(Entity playerEntity);

			int GetLocalClientId();
			void SetLocalClientId(int localClientId);

			void MapClientId(int clientId, Entity playerEntity);

		private:
			int localClientId;

			std::shared_ptr<BidirectionalMap<int, Entity>> clientToPlayerEntityIdMap;
		};
	}
}