#ifndef ECS_H
#define ECS_H

#include "ECSDefines.h"
#include <vector>
#include <map>
#include "Component.h"
#include "System.h"

class ECS
{
public:
	ECS();
	virtual ~ECS();

	template<typename... C>
	EntityHandle addEntity();

	void addSystem(ISystem* sys);
	void removeSystem(ISystem* sys);

	template<typename C>
	void addComponent(EntityHandle entityHandle, C* component);
	template<typename C>
	void removeComponent(EntityHandle entityHandle);

	template<typename C>
	bool hasComponent(EntityHandle entityHandle);
	template<typename C>
	C* getComponent(EntityHandle entityHandle);

private:
	std::vector<ISystem*> systems;
	std::map<ComponentID, std::vector<IComponent*>> components;
	std::vector< EntityPtr > entities;
};

template<typename ...C>
inline EntityHandle ECS::addEntity()
{
	std::vector<IComponent*> comps = { new C()... };
	std::vector<ComponentID> compIDs = { getComponentTypeID<C>()... };

	EntityID id = this->entities.size();
	EntityHandle entityHandle = new std::pair<EntityID, EntityComponents>(id, std::vector<std::pair<ComponentID, ComponentIndex>>());
	for (unsigned int i = 0; i < compIDs.size(); i++)
	{
		ComponentIndex index = this->components[compIDs[i]].size();
		ComponentID id = compIDs[i];
		this->components[id].push_back(comps[i]);
		this->components[id][index]->entityHandle = entityHandle;
		EntityPtr entityPtr = (EntityPtr)entityHandle;
		entityPtr->second.push_back(std::pair<ComponentID, ComponentIndex>(id, index));
	}
	this->entities.push_back((EntityPtr)entityHandle);
	return entityHandle;
}

template<typename C>
inline void ECS::addComponent(EntityHandle entityHandle, C * component)
{
	// This function does not check if the entity already has that type of component.
	ComponentID compID = component->ID;
	ComponentIndex index = this->components[compID].size();
	EntityID entityID = ((EntityPtr)entityHandle)->first;
	this->components[compID].push_back(component);
	this->entities[entityID]->second.push_back(std::pair<ComponentID, ComponentIndex>(compID, index));
}

template<typename C>
inline void ECS::removeComponent(EntityHandle entityHandle)
{
	//TODO: Fix this!
	/*
	ComponentID compID = getComponentTypeID<C>();
	ComponentIndex compIndex = 0;
	EntityComponents& entityComponents = ((EntityPtr)entityHandle)->second;
	entityComponents.erase(std::remove_if(entityComponents.begin(), entityComponents.end(),
		[&compIndex, compID](std::pair<ComponentID, ComponentIndex> x) {
			if (x.first == compID)
			{
				compIndex = x.second;
				return true;
			}
			return false; 
		}),
		entityComponents.end());
	delete this->components[compID][compIndex];
	for (std::map<ComponentID, std::vector<IComponent*>>::iterator it = this->components.begin(); it != this->components.end();)
	{
		if (it->first == compID && compIndex > it->second.size())
		{
			if (it->size() > 1)
			{
				// Remove 
			}
			this->components.erase(it++);
		}
		else
		++it;
	}*/
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
	for (std::pair<ComponentID, ComponentIndex>& c : entityComponents)
		if (c.first == compID)
			return this->components[c.first][c.second];
	return nullptr;
}

#endif
