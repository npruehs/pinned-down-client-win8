#include "pch.h"
#include "Core\EntityManager.h"

using namespace PinnedDownClient::Core;

EntityManager::EntityManager(std::shared_ptr<EventManager> eventManager)
{
	this->eventManager = eventManager;
}

int EntityManager::CreateEntity()
{
	return ++this->nextEntityId;
}

void EntityManager::RemoveEntity(int entityId)
{
	this->removedEntities.insert(entityId);
}

void EntityManager::AddComponent(int entityId, ComponentPtr const & component)
{
	// Get the component type entry in the components map.
	std::map<unsigned long, ComponentMap>::iterator iterator = this->componentMaps.find(component->GetComponentType().getHash());

	if (iterator != this->componentMaps.end())
	{
		ComponentMap & componentMap = iterator->second;

		// Add component.
		componentMap.insert(std::pair<int, ComponentPtr>(entityId, component));
	}
	else
	{
		// Add new component map.
		ComponentMap componentMap = ComponentMap();
		componentMap.insert(std::pair<int, ComponentPtr>(entityId, component));
		this->componentMaps.insert(std::pair<unsigned long, ComponentMap>(component->GetComponentType().getHash(), componentMap));
	}
}

ComponentPtr EntityManager::GetComponent(int entityId, HashedString componentType)
{
	// Lookup component map.
	std::map<unsigned long, ComponentMap>::iterator iterator = this->componentMaps.find(componentType.getHash());

	if (iterator != this->componentMaps.end())
	{
		ComponentMap & componentMap = iterator->second;

		// Lookup component.
		ComponentMap::iterator componentIterator = componentMap.find(entityId);

		if (componentIterator != componentMap.end())
		{
			return ComponentPtr(componentIterator->second);
		}
	}

	// No component found.
	return nullptr;
}

void EntityManager::CleanUpEntities()
{
	for (std::set<int>::iterator entityIterator = this->removedEntities.begin(); entityIterator != this->removedEntities.end(); ++entityIterator)
	{
		int entityId = *entityIterator;

		// Find all attached components.
		for (std::map<unsigned long, ComponentMap>::iterator compMapIterator = this->componentMaps.begin(); compMapIterator != this->componentMaps.end(); ++compMapIterator)
		{
			ComponentMap & componentMap = compMapIterator->second;

			ComponentMap::iterator componentIterator = componentMap.find(entityId);

			if (componentIterator != componentMap.end())
			{
				// Remove component.
				componentMap.erase(componentIterator);
			}
		}
	}

	this->removedEntities.clear();
}