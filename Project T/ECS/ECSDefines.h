#ifndef ECS_DEFINES_H
#define ECS_DEFINES_H

typedef unsigned int ComponentID;
typedef unsigned int EntityID;
class IComponent;
#define EntityComponents std::unordered_map<ComponentID, IComponent*>
#define EntityPtr std::pair<EntityID, EntityComponents>*
#define EntityHandle void*
#define MAX_NUM_COMPONENTS 32

#endif
