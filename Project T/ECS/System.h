#ifndef SYSTEM_H
#define SYSTEM_H

#include "Component.h"

#include <vector>

class ISystem
{
public:
	virtual void init() = 0;
	virtual void update(float dt) = 0;
	virtual void draw() {};

	std::vector<ComponentID> componentRequirements;
};

template<typename... Requirements>
class System : public ISystem
{
public:
	System();
};

template<typename ...Requirements>
System<Requirements...>::System() : ISystem()
{
	// Fill requirements
	this->componentRequirements = { getComponentTypeID<Requirements>()... };
}

#endif
