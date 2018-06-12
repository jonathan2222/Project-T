#ifndef SYSTEM_H
#define SYSTEM_H

#include "Component.h"

#include <vector>

class Renderer;
class ECS;
class ISystem
{
public:
	virtual void init() = 0;
	virtual void update(float dt, const std::vector<EntityHandle>& entities, Renderer* renderer, ECS& ecs) = 0;

	std::vector<ComponentID> componentRequirements;
};

template<typename... Requirements>
class System : public ISystem
{
public:
	System();

	virtual void init() = 0;
	virtual void update(float dt, const std::vector<EntityHandle>& entities, Renderer* renderer, ECS& ecs) = 0;
};

template<typename ...Requirements>
System<Requirements...>::System() : ISystem()
{
	// Fill requirements
	this->componentRequirements = { getComponentTypeID<Requirements>()... };
}

#endif
