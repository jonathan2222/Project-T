#ifndef WORLD_H
#define WORLD_H

#include "Components\ModelComp.h"

class World
{
public:
	World();
	~World();

	ModelComp* getModelComp() const;
	ModelComp* createModelComp() const;

private:
	ModelComp * mc;
};

#endif
