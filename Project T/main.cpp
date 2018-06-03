#include <iostream>

#include "GUI\Display.h"
#include "GLAbstractions\VertexArray.h"
#include "GLAbstractions\IndexBuffer.h"
#include "Shaders\Shader.h"

#include "Maths\Vectors\Vec2.h"
#include "Maths\Vectors\Vec3.h"

int main()
{
	Display display("Project T", 600, 600);

	VertexArray va;
	struct Vertex
	{
		Vec2 pos;
		Vec3 color;
	};
	Vertex data[4] = {
		{ Vec2(-0.5f, 0.5f), Vec3(1.0f, 0.0f, 0.0f) },
		{ Vec2(0.5f, 0.5f), Vec3(0.0f, 1.0f, 0.0f) },
		{ Vec2(0.5f, -0.5f), Vec3(0.0f, 0.0f, 1.0f) },
		{ Vec2(-0.5f, -0.5f), Vec3(1.0f, 1.0f, 1.0f) }
	};
	VertexBuffer vb(data, sizeof(Vertex)*4);
	VertexBufferLayout layout;
	layout.push<float>(2); // Pos
	layout.push<float>(3); // Color
	va.addBuffer(vb, layout);
	unsigned int indices[] = {0, 2, 1, 0, 3, 2};
	IndexBuffer ib(indices, 6);

	Shader shader("./Resources/Shaders/test.fs", "./Resources/Shaders/test.vs");

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	while (display.isOpen())
	{
		display.processEvents();

		display.getWindowPtr()->setActive();

		// Opengl rendering
		shader.bind();
		va.bind();
		ib.bind();
		glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, 0);
		
		display.getWindowPtr()->display();
	}

	return 0;
}