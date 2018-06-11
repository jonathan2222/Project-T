#ifndef ECS_DEFINES_H
#define ECS_DEFINES_H

typedef unsigned int ComponentID;
typedef unsigned int ComponentIndex;
typedef unsigned int EntityID;
#define EntityComponents std::vector<std::pair<ComponentID, ComponentIndex>>
#define EntityPtr std::pair<EntityID, EntityComponents>*
#define EntityHandle void*
#define MAX_NUM_COMPONENTS 32

#endif
