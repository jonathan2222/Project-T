#ifndef WORLD_H
#define WORLD_H

#include "Rendering\Renderer.h"
#include "Rendering\Model.h"

class World
{
public:
	World();
	~World();

	const Renderer* getRenderer() const;

	void addModel(Model* model);
	Model* getModel(unsigned int index) const;
	unsigned int getNumModels() const;

private:
	std::vector<Model*> models;
	Renderer* renderer;
};

#endif
