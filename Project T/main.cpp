#include <iostream>

#include "GUI\Display.h"

#include "Maths\Vectors\Vec2.h"
#include "Maths\Vectors\Vec3.h"


// If Container is used, set it to the class which it will be, this must be done before including ECS!.
class World;
#define Container World

#include "ECS\ECS.h"

#include <random>
#include "Utils\Timer.h"
#include <crtdbg.h>

#include "World.h"

#include "Systems\ColorChangerSys.h"
#include "Systems\CollisionSys.h"
#include "Systems\EnemySys.h"
#include "Systems\PlayerSys.h"
#include "Systems\SpawnerSys.h"
#include "Systems\RenderingSys.h"
#include "Systems\DeleterSys.h"


// ---------------------------------------- MAIN ----------------------------------------

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::srand((unsigned int)std::time(nullptr));

	Display display("Project T", 600, 600);
	World world;
	world.setDisplay(&display);
	
	
	std::cout << "TimeComp: " << getComponentTypeID<TimeComp>() << std::endl;
	std::cout << "PositionComp: " << getComponentTypeID<PositionComp>() << std::endl;
	std::cout << "CollisionComp: " << getComponentTypeID<CollisionComp>() << std::endl;
	std::cout << "ColorComp: " << getComponentTypeID<ColorComp>() << std::endl;
	std::cout << "EnemyComp: " << getComponentTypeID<EnemyComp>() << std::endl;
	std::cout << "RectangleComp: " << getComponentTypeID<RectangleComp>() << std::endl;
	std::cout << "ModelComp: " << getComponentTypeID<ModelComp>() << std::endl;
	std::cout << "PlayerComp: " << getComponentTypeID<PlayerComp>() << std::endl;
	
	
	Model* m = new Model();
	struct Vertex
	{
		Vec2 pos;
		Vec3 color;
	};
	Vertex data[4] = {
		{ Vec2(-1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f) },
	{ Vec2(1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f) },
	{ Vec2(1.0f, -1.0f), Vec3(1.0f, 1.0f, 1.0f) },
	{ Vec2(-1.0f, -1.0f), Vec3(1.0f, 1.0f, 1.0f) }
	};
	VertexBuffer vb(data, sizeof(Vertex) * 4);
	VertexBufferLayout layout;
	layout.push<float>(2); // Pos
	layout.push<float>(3); // Color
	m->va.addBuffer(vb, layout);
	unsigned int indices[] = { 0, 2, 1, 0, 3, 2 };
	m->ib.make(indices, 6);
	world.addModel(m);
	
	ECS ecs;
	
	EntityHandle handle1 = ecs.addEntity<PositionComp, RectangleComp, ModelComp, PlayerComp, ColorComp, CollisionComp>(
		{ new PositionComp(0.0f, 0.0f), new RectangleComp(0.1f, 0.1f), new ModelComp(world.getNumModels()-1), new PlayerComp(), new ColorComp(), new CollisionComp() }
	);
	
	EntityHandle handle2 = ecs.addEntity<PositionComp, RectangleComp, ModelComp, EnemyComp, ColorComp, CollisionComp, TimeComp, SpawnerComp>(
		{ new PositionComp(0.5f, 0.2f), new RectangleComp(0.05f, 0.05f), new ModelComp(world.getNumModels() - 1), new EnemyComp(), new ColorComp(), new CollisionComp(), new TimeComp(), new SpawnerComp() }
	);
	EntityHandle handle3 = ecs.addEntity<PositionComp, RectangleComp, ModelComp, EnemyComp, ColorComp, CollisionComp, TimeComp, SpawnerComp>(
		{ new PositionComp(-0.5f, -0.2f), new RectangleComp(0.2f, 0.2f), new ModelComp(world.getNumModels() - 1),new EnemyComp(), new ColorComp(), new CollisionComp(), new TimeComp(), new SpawnerComp() }
	);

	/*
	
		TODO: Make this faster!!!

	*/
	
	ecs.setContainer<World>(&world);
	ecs.addSystem(new CollisionSys());
	ecs.addSystem(new DeleterSys());
	ecs.addSystem(new SpawnerSys());
	ecs.addSystem(new ColorChangerSys());
	ecs.addSystem(new EnemySys());
	ecs.addSystem(new PlayerSys());
	ecs.addSystem(new RenderingSys());
	
	Shader shader("./Resources/Shaders/test.fs", "./Resources/Shaders/test.vs");

	float dt = 0.0f;
	Timer dtTimer;

	float fps = 0.0f;
	Timer fpsTimer;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	while (display.isOpen())
	{
		display.processEvents();

		display.getWindowPtr()->setActive();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		dtTimer.restart();

		ecs.updateSystems(dt);

		display.setTitleSufix(" dt: " + std::to_string(dt) + ", FPS: " + std::to_string(fps) + ", NumEntities: " + std::to_string(ecs.getNumEntities()));
		
		fpsTimer.update();
		if (fpsTimer.getTime() > 1.0f)
		{
			fps = 1.0f / dt;
			fpsTimer.restart();
		}
		
		dtTimer.stop();
		dt = dtTimer.getTime();
		display.getWindowPtr()->display();
	}

	return 0;
}