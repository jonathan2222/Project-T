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
	for (EntityPtr e : this->entities)
	{
		for(std::unordered_map<ComponentID, IComponent*>::iterator it = e->second.begin(); it != e->second.end(); it++)
			delete it->second;
		delete e;
	}
}

EntityHandle ECS::addEntity(const std::vector<std::pair<ComponentID, IComponent*>>& components)
{
	EntityID id = this->entities.size();
	EntityHandle entityHandle = new std::pair<EntityID, EntityComponents>(id, std::unordered_map<ComponentID, IComponent*>());
	this->entities.push_back((EntityPtr)entityHandle);
	for (unsigned int i = 0; i < components.size(); i++)
	{
		components[i].second->entityHandle = entityHandle;
		EntityPtr entityPtr = (EntityPtr)entityHandle;
		entityPtr->second[components[i].first] = components[i].second;
	}
	return entityHandle;
}

void ECS::addSystem(ISystem * sys)
{
	sys->init();
	this->systems.push_back(sys);
}

void ECS::updateSystems(float dt, Renderer* renderer)
{
	// TODO: Make this faster! Maybe constant amount of components and use a bitset to confirm requirements?
	std::vector<EntityHandle> requiredEntites;
	std::unordered_map<ComponentID, IComponent*>::iterator it;
	for (ISystem* sys : this->systems)
	{
		const std::vector<ComponentID>& requirements = sys->componentRequirements;
		for (EntityPtr entity : this->entities)
		{
			bool hasComponents = true;
			for (unsigned int i = 0; i < requirements.size() && hasComponents; i++)
			{
				const ComponentID& id = requirements[i];
				it = entity->second.find(id);
				if (it == entity->second.end())
					hasComponents = false;
			}
			if(hasComponents)
				requiredEntites.push_back(entity);
		}
		sys->update(dt, requiredEntites, renderer, *this);
		requiredEntites.clear();
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
