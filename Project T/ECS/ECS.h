#ifndef ECS_H
#define ECS_H

#include "ECSDefines.h"

#include <vector>
#include <unordered_map>
#include <map>
#include <bitset>
#include <cassert>

#include "Component.h"
#include "System.h"
#include "../Utils/BitManipulation.h"

#include "ECSContainerDefine.h"

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
	void deleteEntity(EntityHandle handle);

	void addSystem(ISystem* sys);
	void updateSystems(float dt);
	void removeSystem(ISystem* sys);

	template<typename C>
	void addComponent(EntityHandle entityHandle, C* component);
	template<typename C>
	void removeComponent(EntityHandle entityHandle);
	template<typename C>
	void deleteComponent(EntityHandle entityHandle);
	void deleteComponents(EntityHandle entityHandle);

	template<typename C>
	bool hasComponent(EntityHandle entityHandle);
	template<typename C>
	C* getComponent(EntityHandle entityHandle);

	template<typename T>
	void setContainer(T* container);

	unsigned int getNumEntities() const;

	struct Entity
	{
		Entity(EntityID id, const EntityComponents& components) : entityID(id), components(components) { this->componentBitmask = 0; updateBitset(); }
		void updateBitset()
		{
			for (EntityComponents::iterator it = this->components.begin(); it != this->components.end(); it++)
				if (it->second != nullptr)
					Utils::setBit<Bitmask>(this->componentBitmask, it->first);
		}
		EntityComponents components;
		EntityID entityID;
		Bitmask componentBitmask;
		std::vector<ISystem*> systems;
	};

private:
	void addEntityToSystems(EntityHandle handle);
	void removeEntityFromPool(ISystem* sys, EntityHandle handle);

	std::vector<ISystem*> systems;
	std::vector< Entity* > entities;
	Container* container;
};

template<typename ...C>
inline EntityHandle ECS::addEntity()
{
	std::vector<IComponent*> comps = { new C()... };
	std::vector<ComponentID> compIDs = { getComponentTypeID<C>()... };

	EntityID id = this->entities.size();
	EntityHandle entityHandle = new Entity(id, std::unordered_map<ComponentID, IComponent*>());
	for (unsigned int i = 0; i < compIDs.size(); i++)
	{
		ComponentID id = compIDs[i];
#ifdef ECS_DEBUG
		assert(id < MAX_NUM_COMPONENTS);
#endif
		comps[i]->entityHandle = entityHandle;
		Entity* entityPtr = (Entity*)entityHandle;
		entityPtr->components[id] = comps[i];
	}
	((Entity*)entityHandle)->updateBitset();
	this->entities.push_back((Entity*)entityHandle);
	addEntityToSystems(entityHandle);
	return entityHandle;
}

template<typename... C>
EntityHandle ECS::addEntity(const std::vector<IComponent*>& components)
{
	std::vector<ComponentID> compIDs = { getComponentTypeID<C>()... };

	EntityID id = this->entities.size();
	EntityHandle entityHandle = new Entity(id, std::unordered_map<ComponentID, IComponent*>());
	for (unsigned int i = 0; i < compIDs.size(); i++)
	{
		ComponentID id = compIDs[i];
#ifdef ECS_DEBUG
		assert(id < MAX_NUM_COMPONENTS);
#endif
		components[i]->entityHandle = entityHandle;
		Entity* entityPtr = (Entity*)entityHandle;
		entityPtr->components[id] = components[i];
	}
	((Entity*)entityHandle)->updateBitset();
	this->entities.push_back((Entity*)entityHandle);
	addEntityToSystems(entityHandle);
	return entityHandle;
}

template<typename C>
inline void ECS::addComponent(EntityHandle entityHandle, C * component)
{
	// Does not check if entity already has this type of component!
	ComponentID compID = component->ID;
#ifdef ECS_DEBUG
	assert(compID < MAX_NUM_COMPONENTS);
#endif
	EntityID entityID = ((Entity*)entityHandle)->entityID;
	component->entityHandle = entityHandle;
	Entity* entity = this->entities[entityID];
	entity->components[compID] = component;
	Utils::setBit<Bitmask>(entity->componentBitmask, compID);
}

template<typename C>
inline void ECS::removeComponent(EntityHandle entityHandle)
{
	// This dose not delete the element in the unorderd_map, but only deletes the pointer and set it to nullptr.
	ComponentID compID = getComponentTypeID<C>();
#ifdef ECS_DEBUG
	assert(compID < MAX_NUM_COMPONENTS);
#endif
	Entity* entity = (Entity*)entityHandle;
	EntityID eID = entity->entityID;
	EntityComponents& entityComponents = entity->components;
	EntityComponents::iterator it = entityComponents.find(compID);
	if (it != entityComponents.end())
	{
		Bitmask entityBitmask = entity->componentBitmask;
		entity->systems.erase(std::remove_if(entity->systems.begin(), entity->systems.end(), [entityHandle, &entityBitmask, &eID](ISystem* isys) {
			Bitmask sysBitmask = isys->componentBitmask;
			if ((entityBitmask & sysBitmask) == sysBitmask)
			{
				isys->hasInitialized[eID] = false;
				removeEntityFromPool(isys, entityHandle);
				return true;
			}
			else return false;
		}), entity->systems.end());

		Utils::clearBit<Bitmask>(((Entity*)it->second->entityHandle)->componentBitmask, compID);
		delete it->second;
		it->second = nullptr;
		return;
	}
	
}

template<typename C>
inline void ECS::deleteComponent(EntityHandle entityHandle)
{
	ComponentID compID = getComponentTypeID<C>();
#ifdef ECS_DEBUG
	assert(compID < MAX_NUM_COMPONENTS);
#endif
	EntityID eID = entity->entityID;
	EntityComponents& entityComponents = (((Entity*))entityHandle)->components;
	EntityComponents::iterator it = entityComponents.find(compID);
	if (it != entityComponents.end())
	{
		Bitmask entityBitmask = entity->componentBitmask;
		entity->systems.erase(std::remove_if(entity->systems.begin(), entity->systems.end(), [entityHandle, &entityBitmask, &eID](ISystem* isys) {
			Bitmask sysBitmask = isys->componentBitmask;
			if ((entityBitmask & sysBitmask) == sysBitmask)
			{
				isys->hasInitialized[eID] = false;
				removeEntityFromPool(isys, entityHandle);
				return true;
			}
			else return false;
		}), entity->systems.end());

		Utils::clearBit<Bitmask>(((Entity*)it->second->entityHandle)->componentBitmask, compID);
		delete it->second;
		entityComponents.erase(it);
		return;
	}
}

template<typename C>
inline bool ECS::hasComponent(EntityHandle entityHandle)
{
	ComponentID compID = getComponentTypeID<C>();
#ifdef ECS_DEBUG
	assert(compID < MAX_NUM_COMPONENTS);
#endif
	return Utils::isBitSet<Bitmask>(((Entity*)entityHandle)->componentBitmask, compID);
}

template<typename C>
inline C* ECS::getComponent(EntityHandle entityHandle)
{
	ComponentID compID = getComponentTypeID<C>();
#ifdef ECS_DEBUG
	assert(compID < MAX_NUM_COMPONENTS);
#endif
	EntityComponents& entityComponents = ((Entity*)entityHandle)->components;
	return (C*)entityComponents[compID];
}

template<typename T>
inline void ECS::setContainer(T * container)
{
	this->container = (Container*)container;
}

#endif
