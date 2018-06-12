#ifndef ECS_H
#define ECS_H

#include "ECSDefines.h"
#include <vector>
#include <unordered_map>
#include <map>
#include "Component.h"
#include "System.h"

class Renderer;
class ECS
{
public:
	ECS();
	virtual ~ECS();

	template<typename... C>
	EntityHandle addEntity();
	template<typename... C>
	EntityHandle addEntity(const std::vector<IComponent*>& components);

	EntityHandle addEntity(const std::vector<std::pair<ComponentID, IComponent*>>& components);

	void addSystem(ISystem* sys);
	void updateSystems(float dt, Renderer* renderer = nullptr);
	void removeSystem(ISystem* sys);

	template<typename C>
	void addComponent(EntityHandle entityHandle, C* component);
	template<typename C>
	void removeComponent(EntityHandle entityHandle);
	template<typename C>
	void deleteComponent(EntityHandle entityHandle);

	template<typename C>
	bool hasComponent(EntityHandle entityHandle);
	template<typename C>
	C* getComponent(EntityHandle entityHandle);

private:
	std::vector<ISystem*> systems;
	std::vector< EntityPtr > entities;
};

template<typename ...C>
inline EntityHandle ECS::addEntity()
{
	std::vector<IComponent*> comps = { new C()... };
	std::vector<ComponentID> compIDs = { getComponentTypeID<C>()... };

	EntityID id = this->entities.size();
	EntityHandle entityHandle = new std::pair<EntityID, EntityComponents>(id, std::unordered_map<ComponentID, IComponent*>());
	this->entities.push_back((EntityPtr)entityHandle);
	for (unsigned int i = 0; i < compIDs.size(); i++)
	{
		ComponentID id = compIDs[i];
		comps[i]->entityHandle = entityHandle;
		EntityPtr entityPtr = (EntityPtr)entityHandle;
		entityPtr->second[id] = comps[i];
	}
	return entityHandle;
}

template<typename... C>
EntityHandle ECS::addEntity(const std::vector<IComponent*>& components)
{
	std::vector<ComponentID> compIDs = { getComponentTypeID<C>()... };

	EntityID id = this->entities.size();
	EntityHandle entityHandle = new std::pair<EntityID, EntityComponents>(id, std::unordered_map<ComponentID, IComponent*>());
	this->entities.push_back((EntityPtr)entityHandle);
	for (unsigned int i = 0; i < compIDs.size(); i++)
	{
		ComponentID id = compIDs[i];
		components[i]->entityHandle = entityHandle;
		EntityPtr entityPtr = (EntityPtr)entityHandle;
		entityPtr->second[id] = components[i];
	}
	return entityHandle;
}

template<typename C>
inline void ECS::addComponent(EntityHandle entityHandle, C * component)
{
	// Does not check if entity already has this type of component!
	ComponentID compID = component->ID;
	EntityID entityID = ((EntityPtr)entityHandle)->first;
	component->entityHandle = entityHandle;
	this->entities[entityID]->second[compID] = component;
}

template<typename C>
inline void ECS::removeComponent(EntityHandle entityHandle)
{
	// This dose not delete the element in the unorderd_map, but only deletes the pointer and set it to nullptr.
	ComponentID compID = getComponentTypeID<C>();
	EntityComponents& entityComponents = ((EntityPtr)entityHandle)->second;
	for (std::unordered_map<ComponentID, IComponent*>::iterator it = entityComponents.begin(); it != entityComponents.end(); it++)
		if (it->first == compID)
		{
			delete it->second;
			it->second = nullptr;
			return;
		}
}

template<typename C>
inline void ECS::deleteComponent(EntityHandle entityHandle)
{
	ComponentID compID = getComponentTypeID<C>();
	EntityComponents& entityComponents = ((EntityPtr)entityHandle)->second;
	for (std::unordered_map<ComponentID, IComponent*>::iterator it = entityComponents.begin(); it != entityComponents.end(); it++)
	{
		if (it->first == compID)
		{
			delete it->second;
			entityComponents.erase(it);
			return;
		}
	}
}

template<typename C>
inline bool ECS::hasComponent(EntityHandle entityHandle)
{
	return getComponent<C>(entityHandle) != nullptr;
}

template<typename C>
inline C* ECS::getComponent(EntityHandle entityHandle)
{
	ComponentID compID = getComponentTypeID<C>();
	EntityComponents& entityComponents = ((EntityPtr)entityHandle)->second;
	std::unordered_map<ComponentID, IComponent*>::iterator it = entityComponents.find(compID);
	if (it != entityComponents.end())
		return (C*)it->second;
	return nullptr;
}

#endif
