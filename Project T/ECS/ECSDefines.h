#ifndef ECS_DEFINES_H
#define ECS_DEFINES_H

typedef unsigned int ComponentID;
typedef unsigned int EntityID;
typedef unsigned int SystemID;
class IComponent;
#define EntityComponents std::unordered_map<ComponentID, IComponent*>
#define EntityHandle void*
#define Bitmask unsigned long long
#define MAX_NUM_COMPONENTS sizeof(Bitmask)*8

#define ECS_DEBUG

#endif
