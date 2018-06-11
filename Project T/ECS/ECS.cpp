#include "ECS.h"

ECS::ECS()
{
}

ECS::~ECS()
{
	// Delete systems
	for (ISystem* sys : this->systems)
		delete sys;
	// Delete entites
	for (EntityPtr e : this->entities)
		delete e;
	// Delete components
	for (std::map<ComponentID, std::vector<IComponent*>>::iterator it = this->components.begin(); it != this->components.end(); it++)
		for (IComponent* c : it->second)
			delete c;
}

void ECS::addSystem(ISystem * sys)
{
}

void ECS::removeSystem(ISystem * sys)
{
}
