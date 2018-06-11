#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <vector>

#include "System.h"

class SystemManager
{
public:
	SystemManager();
	virtual ~SystemManager();
	
	template<typename T, typename ...TArgs>
	void addSystem(TArgs... args);
	void addSystem(ISystem* sys);
	void update(const std::vector<Entity*>& entities);

private:
	std::vector<ISystem*> systems;
};

template<typename T, typename ...TArgs>
inline void SystemManager::addSystem(TArgs... args)
{
	this->systems.emplace_back(new T(std::forward(args)...));
}

#endif
