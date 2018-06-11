#include "Entity.h"

Entity::Entity()
{
	this->id = getNextID();
	this->componentArray.fill(nullptr);
}

Entity::~Entity()
{
	for (BaseComponent* bc : this->componentArray)
		if(bc != nullptr)
			delete bc;
}

EntityID Entity::getNextID()
{
	static EntityID idCounter = 0;
	return idCounter++;;
}
