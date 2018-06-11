#ifndef COMPONENT_H
#define COMPONENT_H

#include <cstddef>

#include "ECSDefines.h"


class IComponent
{
public:
	EntityHandle entityHandle;
};

inline ComponentID getComponentTypeID()
{
	static ComponentID typeID = 0;
	return typeID++;
}

template<typename T>
inline const ComponentID getComponentTypeID()
{
	static ComponentID typeID = getComponentTypeID();
	return typeID;
}

template<typename T>
class Component : public IComponent
{
public:
	static const std::size_t SIZE;
	static const ComponentID ID;
};

template<typename T>
const std::size_t Component<T>::SIZE(sizeof(T));

template<typename T>
const ComponentID Component<T>::ID(getComponentTypeID<T>());

#endif
