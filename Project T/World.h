#ifndef WORLD_H
#define WORLD_H

#include "Rendering\Renderer.h"
#include "Rendering\Model.h"

class Display;
class World
{
public:
	World();
	~World();

	const Renderer* getRenderer() const;

	void addModel(Model* model);
	Model* getModel(unsigned int index) const;
	unsigned int getNumModels() const;

	void setDisplay(Display* display);
	Display* getDisplay();

private:
	std::vector<Model*> models;
	Renderer* renderer;
	Display* display;
};

#endif
