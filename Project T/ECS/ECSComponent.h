#ifndef ECS_COMPONENT_H
#define ECS_COMPONENT_H

/*

	Code from thebennybox.
	Youtube url: https://www.youtube.com/watch?v=Y6he35HfDmA

*/

#include <vector>
typedef unsigned int uint32;
typedef unsigned char uint8;

typedef void* EntityHandle;
#define NULL_ENTITY_HANDLE nullptr

struct BaseECSComponent;
typedef uint32 (*ECSComponentCreateFunction)(std::vector<uint8>& memory, EntityHandle entity, BaseECSComponent* comp);
typedef void (*ECSComponentFreeFunction)(BaseECSComponent* comp); // ECSComponentFreeFunction f; <=> void (*f)(BaseECSComponent* comp)

struct BaseECSComponent
{
	static uint32 getNextId();
	EntityHandle entity = NULL_ENTITY_HANDLE;
};

template<typename T>
struct ECSComponent
{
	static const ECSComponentCreateFunction CREATE_FUNCTION;
	static const ECSComponentFreeFunction FREE_FUNCTION;
	static const uint32 ID;
	static const size_t SIZE;
};

template<typename Component>
uint32 ECSComponentCreate(std::vector<uint8>& memory, EntityHandle entity, BaseECSComponent* comp)
{
	uint32 index = memory.size();
	memory.resize(index + Component::SIZE);
	Component* component = new(&memory[index])Component(*(Component*)comp);
	component->entity = entity;
	return index;
}

template<typename Component>
void ECSComponentFree(BaseECSComponent* comp)
{
	Component* component = (Component*)comp;
	component->~Component();
}

template<typename T>
const uint32 ECSComponent<T>::ID(BaseECSComponent::getNextId());

template<typename T>
const size_t ECSComponent<T>::SIZE(sizeof(T));

template<typename T>
const ECSComponentCreateFunction ECSComponent<T>::CREATE_FUNCTION(ECSComponentCreate<T>);

template<typename T>
const ECSComponentFreeFunction ECSComponent<T>::FREE_FUNCTION(ECSComponentFree<T>);

/* Example code
struct TestECSComponent : public ECSComponent<TestECSComponent>
{
	float a;
	float b;
};
*/

#endif
