#include "ECS.h"

ECS::ECS()
{
}

ECS::~ECS()
{
	// Delete systems
	for (ISystem* sys : this->systems)
		delete sys;
	// Delete entites and its components.
	for (Entity* e : this->entities)
	{
		for(EntityComponents::iterator it = e->components.begin(); it != e->components.end(); it++)
			delete it->second;
		delete e;
	}
}

void ECS::addSystem(ISystem * sys)
{
	std::vector<EntityHandle> requiredEntitiesInit;
	Bitmask sysBitmask = sys->componentBitmask;
	for (Entity* entity : this->entities)
	{
		Bitmask entityBitmask = entity->componentBitmask;
		if ((entityBitmask & sysBitmask) == sysBitmask)
		{
			entity->systems.push_back(sys);
			sys->hasInitialized[entity->entityID] = true;
			requiredEntitiesInit.push_back(entity);
		}
	}
	sys->init(requiredEntitiesInit, *this, this->container);
	this->systems.push_back(sys);
}

void ECS::updateSystems(float dt, Renderer* renderer)
{
	static std::vector<EntityHandle> requiredEntities;
	static std::vector<EntityHandle> requiredEntitiesInit;
	std::unordered_map<ComponentID, IComponent*>::iterator it;
	for (ISystem* sys : this->systems)
	{
		Bitmask sysBitmask = sys->componentBitmask;
		unsigned int entitySize = this->entities.size();
		for (unsigned int i = 0; i < entitySize; i++)
		{
			Entity* entity = this->entities[i];
			Bitmask entityBitmask = entity->componentBitmask;
			if ((entityBitmask & sysBitmask) == sysBitmask)
			{
				requiredEntities.push_back(entity);
				std::unordered_map<EntityID, bool>::iterator it = sys->hasInitialized.find(entity->entityID);
				if (it == sys->hasInitialized.end() || it->second == false)
				{
					if (it == sys->hasInitialized.end())
						sys->hasInitialized[entity->entityID] = true;
					else
						it->second = true;
					entity->systems.push_back(sys);
					requiredEntitiesInit.push_back(entity);
				}
			}
		}

		if(!requiredEntitiesInit.empty())
			sys->init(requiredEntitiesInit, *this, this->container);
		sys->update(dt, requiredEntities, renderer, *this, this->container);
		requiredEntities.clear();
		requiredEntitiesInit.clear();
	}
}

void ECS::removeSystem(ISystem * sys)
{
	for (ISystem* s : this->systems)
		if (s == sys)
		{
			delete sys;
			return;
		}
}
