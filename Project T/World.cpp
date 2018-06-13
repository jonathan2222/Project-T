#include "World.h"

#include "Maths\Vectors\Vec2.h"
#include "Maths\Vectors\Vec3.h"

World::World()
{
	this->renderer = new Renderer();
}

World::~World()
{
	delete this->renderer;
	for (Model* m : this->models)
		delete m;
}

const Renderer * World::getRenderer() const
{
	return this->renderer;
}

void World::addModel(Model * model)
{
	this->models.push_back(model);
}

Model * World::getModel(unsigned int index) const
{
	return this->models[index];
}

unsigned int World::getNumModels() const
{
	return this->models.size();
}

void World::setDisplay(Display * display)
{
	this->display = display;
}

Display * World::getDisplay()
{
	return this->display;
}
