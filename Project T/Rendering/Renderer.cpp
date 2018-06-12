#include "Renderer.h"

Renderer::Renderer()
{
	this->defaultShader = new Shader("./Resources/Shaders/test.fs", "./Resources/Shaders/test.vs");
}

Renderer::~Renderer()
{
	delete this->defaultShader;
}

void Renderer::draw(const VertexArray & va, const IndexBuffer & ib, const Shader * shader) const
{
	if (shader != nullptr)
		shader->bind();
	else
		this->defaultShader->bind();
	va.bind();
	ib.bind();
	glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, 0);
}
