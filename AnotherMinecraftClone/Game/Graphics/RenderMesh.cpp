#include "RenderMesh.h"

#include "..\..\OpenGL/glew/include/GL/glew.h"

using namespace Game::Graphics;

void RenderMesh::Upload()
{
	if (IsUploaded || Indicies.size() == 0 || Vertacies.size() == 0)
		return;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * Indicies.size(), &Indicies[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertacies.size(), &Vertacies[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (const void*)offsetof(Vertex, Vertex::iPosition));
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (const void*)offsetof(Vertex, Vertex::iNormal));
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (const void*)offsetof(Vertex, Vertex::iUV));

	for (int i = 0; i < 3; i++)
	{
		glEnableVertexAttribArray(i);
	}

	IsUploaded = true;
}

void RenderMesh::Draw()
{
	Upload();

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glDrawElements(RenderMode, Indicies.size(), GL_UNSIGNED_INT, NULL);
}

RenderMesh::RenderMesh()
{

}

RenderMesh::~RenderMesh()
{
	if (IsUploaded)
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &ibo);
		glDeleteBuffers(1, &vbo);
	}
}