#pragma once

#include <map>

#include "EntityManager.h"

using namespace PinnedDownCore;

namespace PinnedDownClient
{
	namespace Util
	{
		class ClientIdMapping
		{
		public:
			bool IsLocalPlayer(Entity playerEntity);

			int GetLocalClientId();
			void SetLocalClientId(int localClientId);

			void MapClientId(int clientId, Entity playerEntity);

		private:
			int localClientId;

			std::map<int, Entity> clientToPlayerEntityIdMap;
			std::map<Entity, int> playerEntityToClientIdMap;
		};
	}
}