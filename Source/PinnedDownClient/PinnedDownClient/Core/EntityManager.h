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
	namespace Core
	{
		class EntityManager
		{
		public:
			EntityManager(std::shared_ptr<EventManager> eventManager);

			int CreateEntity();
			void RemoveEntity(int entityId);

			void AddComponent(int entityId, ComponentPtr const & component);
			ComponentPtr GetComponent(int entityId, HashedString componentType);

			template <class T> T& GetComponent(int entityId, HashedString componentType)
			{
				ComponentPtr p = this->GetComponent(entityId, componentType);

				if (p != nullptr) {
					EntityComponent & entityComponent = *p;
					T & reference = static_cast<T&>(entityComponent);
					return reference;
				}

				return nullptr;
			}

			void CleanUpEntities();

		private:
			std::shared_ptr<EventManager> eventManager;

			// Maps that are mapping entity ids to specific components.
			std::map<unsigned long, ComponentMap> componentMaps;

			// Ids of all entities that have been removed in this tick.
			std::set<int> removedEntities;

			// Id that will be assigned to the next entity created.
			int nextEntityId;
		};
	}
}
