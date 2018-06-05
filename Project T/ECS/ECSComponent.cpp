#include "ECSComponent.h"

uint32 BaseECSComponent::getNextId()
{
	static uint32 id = 0;
	return id++;
}
