#ifndef COMPONENT_H
#define COMPONENT_H

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
	static const unsigned int SIZE;
	static const ComponentID ID;
};

template<typename T>
const unsigned int Component<T>::SIZE(sizeof(T));

template<typename T>
const ComponentID Component<T>::ID(getComponentTypeID<T>());

#endif
