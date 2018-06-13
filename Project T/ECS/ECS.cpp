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

void ECS::deleteEntity(EntityHandle handle)
{
	unsigned int numEntities = this->entities.size();
	for (unsigned int i = 0; i < numEntities; i++)
	{
		Entity* entity = this->entities[i];
		if (entity == handle)
		{
			deleteComponents(entity);

			if (numEntities > 1)
			{
				Entity* other = this->entities[numEntities - 1];
				this->entities[i] = other;
				this->entities[numEntities - 1] = nullptr;

				// Update Systems' initializeing list.
				Bitmask entityBitmask = other->componentBitmask;
				for (ISystem* sys : other->systems)
				{
					Bitmask sysBitmask = sys->componentBitmask;
					if ((entityBitmask & sysBitmask) == sysBitmask)
					{
						sys->hasInitialized[numEntities - 1] = false;
						sys->hasInitialized[i] = false;
					}
				}
				other->entityID = i;
			}
			this->entities.pop_back();
			delete entity;
			return;
		}
	}
}

void ECS::addSystem(ISystem * sys)
{
	Bitmask sysBitmask = sys->componentBitmask;
	for (Entity* entity : this->entities)
	{
		Bitmask entityBitmask = entity->componentBitmask;
		if ((entityBitmask & sysBitmask) == sysBitmask)
		{
			entity->systems.push_back(sys);
			sys->hasInitialized[entity->entityID] = true;
			sys->entityPool.push_back(entity);
		}
	}
	sys->init(sys->entityPool, *this, this->container);
	this->systems.push_back(sys);
}

void ECS::updateSystems(float dt)
{
	for (ISystem* sys : this->systems)
		sys->update(dt, sys->entityPool, *this, this->container);
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

void ECS::deleteComponents(EntityHandle entityHandle)
{
	Entity* entity = (Entity*)entityHandle;
	EntityID eID = entity->entityID;
	EntityComponents& entityComponents = entity->components;
	EntityComponents::iterator it = entityComponents.begin();
	Bitmask& entityBitmask = entity->componentBitmask;
	for (; it != entityComponents.end(); it++)
	{
		for (ISystem* sys : entity->systems)
		{
			Bitmask sysBitmask = sys->componentBitmask;
			if ((entityBitmask & sysBitmask) == sysBitmask)
			{
				sys->hasInitialized[eID] = false;
				removeEntityFromPool(sys, entityHandle);
			}
		}
		delete it->second;
	}
	entityBitmask = 0;
	entityComponents.clear();
}

unsigned int ECS::getNumEntities() const
{
	return this->entities.size();
}

void ECS::addEntityToSystems(EntityHandle handle)
{
	static std::vector<EntityHandle> requiredEntitiesInit;
	Entity* entity = (Entity*)handle;
	requiredEntitiesInit.push_back(handle);
	Bitmask entityBitmask = entity->componentBitmask;
	for (ISystem* sys : this->systems)
	{
		Bitmask sysBitmask = sys->componentBitmask;
		if ((entityBitmask & sysBitmask) == sysBitmask)
		{
			sys->hasInitialized[entity->entityID] = true;
			entity->systems.push_back(sys);
			sys->entityPool.push_back(handle);
			sys->init(requiredEntitiesInit, *this, this->container);
		}
	}
	requiredEntitiesInit.clear();
}

void ECS::removeEntityFromPool(ISystem* sys, EntityHandle handle)
{
	unsigned int size = sys->entityPool.size();
	for (unsigned int i = 0; i < size; i++)
	{
		if (sys->entityPool[i] == handle)
		{
			if(i+1 < size)
				sys->entityPool[i] = sys->entityPool[size - 1];
			sys->entityPool.pop_back();
			return;
		}
	}
}
