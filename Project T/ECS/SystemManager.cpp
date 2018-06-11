#include "SystemManager.h"

SystemManager::SystemManager()
{
}

SystemManager::~SystemManager()
{
	for (ISystem* sys : this->systems)
		delete sys;
}

void SystemManager::addSystem(ISystem* sys)
{
	this->systems.push_back(sys);
}

void SystemManager::update(const std::vector<Entity*>& entities)
{
	/*for (ISystem* sys : this->systems)
	{
		sys->meetsTheRequirements(entities);
	}*/
}
