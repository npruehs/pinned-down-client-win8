#pragma once

#include <map>
#include <set>
#include <memory>
#include "EventManager.h"
#include "EntityComponent.h"

typedef std::shared_ptr<PinnedDownClient::Core::EntityComponent> ComponentPtr;
typedef std::map<int, ComponentPtr> ComponentMap;

namespace PinnedDownClient
{
	class PinnedDownGame;

	namespace Core
	{
		class EntityManager
		{
		public:
			EntityManager(PinnedDownClient::PinnedDownGame* game);

			int CreateEntity();
			void RemoveEntity(int entityId);

			void AddComponent(int entityId, ComponentPtr const & component);
			ComponentPtr GetComponent(int entityId, HashedString componentType);

			template <class T> std::shared_ptr<T> GetComponent(int entityId, HashedString componentType)
			{
				ComponentPtr p = this->GetComponent(entityId, componentType);
				return std::static_pointer_cast<T>(p);
			}

			void CleanUpEntities();

		private:
			PinnedDownClient::PinnedDownGame* game;

			// Maps that are mapping entity ids to specific components.
			std::map<unsigned long, ComponentMap> componentMaps;

			// Ids of all entities that have been removed in this tick.
			std::set<int> removedEntities;

			// Id that will be assigned to the next entity created.
			int nextEntityId;
		};
	}
}
