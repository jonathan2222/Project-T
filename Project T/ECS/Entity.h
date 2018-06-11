#ifndef ENTITY_H
#define ENTITY_H

#include "ECSDefines.h"
#include <bitset>
#include <memory>
#include <array>

#include "Component.h"

class Entity
{
public:
	Entity();
	virtual ~Entity();

	template<typename T, typename... TArgs>
	bool addComponent(TArgs&&... tArgs);
	template<typename T>
	bool hasComponent() const;
	template<typename T>
	T& getComponent();
	template<typename T>
	bool removeComponenet();

	EntityID id;

private:
	std::bitset<MAX_NUM_COMPONENTS> bitset;
	std::array<IComponent*, MAX_NUM_COMPONENTS> componentArray;

	EntityID getNextID();
};

template<typename T, typename ...TArgs>
inline bool Entity::addComponent(TArgs && ... tArgs)
{
	// Add component if entity does not have that one.
	if (!this->bitset[getComponentTypeID<T>()])
	{
		T* c(new T(std::forward<TArgs>(tArgs)...));
		//c->entityPtr = this;
		this->bitset[getComponentTypeID<T>()] = true;
		this->componentArray[getComponentTypeID<T>()] = c;
		return true;
	}
	return false;
}

template<typename T>
inline bool Entity::hasComponent() const
{
	return this->bitset[getComponentTypeID<T>()];
}

template<typename T>
inline T & Entity::getComponent()
{
	return *(T*)this->componentArray[getComponentTypeID<T>()];
}

template<typename T>
inline bool Entity::removeComponenet()
{
	if (hasComponenet<T>())
	{
		delete this->componentArray[getComponentTypeID<T>()];
		this->componentArray[getComponentTypeID<T>()] = nullptr;
		this->bitset[getComponentTypeID<T>()] = false;
		return true;
	}
	return false;
}

#endif
