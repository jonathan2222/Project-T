#ifndef SYSTEM_H
#define SYSTEM_H

#include "Component.h"

#include "ECSDefines.h"
#include "../Utils/BitManipulation.h"

#include "ECSContainerDefine.h"

#include <vector>

class Renderer;
class ECS;
class Entity;
class ISystem
{
public:
	virtual void init(const std::vector<EntityHandle>& entities, ECS& ecs, Container* container = nullptr) = 0;
	virtual void update(float dt, const std::vector<EntityHandle>& entities, ECS& ecs, Container* container = nullptr) = 0;

	std::vector<ComponentID> componentRequirements;
	Bitmask componentBitmask;
	SystemID systemID;
	std::unordered_map<EntityID, bool> hasInitialized;
	std::vector<EntityHandle> entityPool;
};

template<typename... Requirements>
class System : public ISystem
{
public:
	System();

	virtual void init(const std::vector<EntityHandle>& entities, ECS& ecs, Container* container = nullptr) = 0;
	virtual void update(float dt, const std::vector<EntityHandle>& entities, ECS& ecs, Container* container = nullptr) = 0;
};

template<typename ...Requirements>
System<Requirements...>::System() : ISystem()
{
	// Fill requirements
	this->componentRequirements = { getComponentTypeID<Requirements>()... };
	// Initialize bitset
	this->componentBitmask = 0;
	for (ComponentID compID : this->componentRequirements)
	{
		Utils::setBit<Bitmask>(this->componentBitmask, compID);
	}
	// Set ID;
	static SystemID counter = 0;
	this->systemID = counter++;
}

#endif
