#include "World.h"
#include "Graphics/RenderMesh.h"
#include "Graphics/RenderShader.h"
#include "..\OpenGL/glew/include/GL/glew.h"
#include <iostream>
#include "glm/glm/vec3.hpp"
#include "ChunkDataGenerator.h"
#include "Core.h"

using namespace Game;
using namespace Game::Graphics;

RenderShader* Chunk::ChunkShader = nullptr;

void Chunk::LoadChunkShader()
{
	if (ChunkShader != nullptr)
		return;

	ShaderSource Vertex;
	ShaderSource Fragment;

	if (true)
	{
		Vertex = ShaderSource(Game::ReadFileText("E:/Projects/Build/Shaders/Terrain/vertex.glsl"),GL_VERTEX_SHADER);
		Fragment = ShaderSource(Game::ReadFileText("E:/Projects/Build/Shaders/Terrain/fragment.glsl"), GL_FRAGMENT_SHADER);
	}
	else
	{
		Vertex = ShaderSource(Game::ReadFileText("vertex.glsl"), GL_VERTEX_SHADER);
		Fragment = ShaderSource(Game::ReadFileText("fragment.glsl"), GL_FRAGMENT_SHADER);
	}

	ChunkShader = new RenderShader();

	ChunkShader->Sources.push_back(Vertex);
	ChunkShader->Sources.push_back(Fragment);

	ChunkShader->CompileSources();

	ChunkShader->Use();
}

Chunk::Chunk(int X, int Z, World* world)
{
	Mesh = new RenderMesh();

	Mesh->RenderMode = GL_QUADS;

	this->_PositionX = X;
	this->_PositionZ = Z;

	this->world = world;
}

Chunk::~Chunk()
{
	delete Mesh;
}

bool Chunk::OtherChunkReady(int X, int Z)
{
	bool exists = world->ChunkExists(X, Z);

	if (exists)
	{
		return world->GetChunk(X, Z)->DataGenerated;
	}

	return false;
}

void Chunk::GenerateChunk()
{
	if (!DataGenerated)
	{
		GenerateData();

		return;
	}

	if (!MeshGenerated)
	{
		if (!(
			OtherChunkReady(_PositionX + CHUNKWIDTH, _PositionZ) &&
			OtherChunkReady(_PositionX - CHUNKWIDTH, _PositionZ) &&
			OtherChunkReady(_PositionX, _PositionZ + CHUNKWIDTH) &&
			OtherChunkReady(_PositionX, _PositionZ - CHUNKWIDTH)
			))

			return;

		GenerateMesh();

		return;
	}
}

void Chunk::GenerateData()
{
	ChunkDataGenerator DataGenerator;

	for (int x = 0; x < CHUNKWIDTH; x++)
	{
		for (int z = 0; z < CHUNKWIDTH; z++)
		{
			int RX = _PositionX + x;
			int RZ = _PositionZ + z;

			float height = (sin(RX / 20.0f) + cos(RZ / 20.0f));

			height *= 20.0f;

			height += 40;

			for (int y = 0; y < CHUNKHEIGHT; y++)
			{
				BlockBuffer[BLOCKINDEX] = { (uint8_t)(y < height ? 1 : 0) };

				if (BlockBuffer[BLOCKINDEX].ID != 0)
					BlockQue.push_back({ x, y, z });
			}
		}
	}

	DataGenerated = true;
}

void Chunk::GenerateMesh()
{
	for (glm::ivec3 block : BlockQue)
	{
		GenerateMeshForBlock(block.x, block.y, block.z);
	}

	MeshGenerated = true;
}

void Chunk::GenerateMeshForBlock(int x, int y, int z)
{
	Block& CurrentBlock = BlockBuffer[BLOCKINDEX];

	if (CurrentBlock.ID == 0)
		return;

	for (int i = 0; i < 6; ++i)
	{
		int bits = (1 << (i >> 1));

		int xo = (bits >> 0) & 1;
		int yo = (bits >> 1) & 1;
		int zo = (bits >> 2) & 1;

		if ((i & ~1) == i)
		{
			xo *= -1;
			yo *= -1;
			zo *= -1;
		}

		GenerateFaceIfNeeded(x, y, z, xo, yo, zo);
	}
}

void Chunk::GenerateFaceIfNeeded(int x, int y, int z, int ox, int oy, int oz)
{
	Block CurrentBlock = GetBlock(x, y, z);

	Block Check = GetBlock(x + ox, y + oy, z + oz);

	if (Check.ID != 0)
		return;

	glm::vec3 BasePosition = glm::vec3(x,y,z) + glm::vec3((int)_PositionX, 0, (int)_PositionZ);

	if (ox != 0)
	{
		int offset = ox == 1 ? 1 : 0;

		Mesh->Vertacies.push_back({ BasePosition + glm::vec3(offset,0,0) });
		Mesh->Vertacies.push_back({ BasePosition + glm::vec3(offset,1,0) });
		Mesh->Vertacies.push_back({ BasePosition + glm::vec3(offset,1,1) });
		Mesh->Vertacies.push_back({ BasePosition + glm::vec3(offset,0,1) });

		AddQuad(offset == 1);
	}

	if (oy != 0)
	{
		int offset = oy == 1 ? 1 : 0;

		Mesh->Vertacies.push_back({ BasePosition + glm::vec3(0,offset,0) });
		Mesh->Vertacies.push_back({ BasePosition + glm::vec3(1,offset,0) });
		Mesh->Vertacies.push_back({ BasePosition + glm::vec3(1,offset,1) });
		Mesh->Vertacies.push_back({ BasePosition + glm::vec3(0,offset,1) });

		AddQuad(offset == -1);
	}

	if (oz != 0)
	{
		int offset = oz == 1 ? 1 : 0;

		Mesh->Vertacies.push_back({ BasePosition + glm::vec3(0,0,offset) });
		Mesh->Vertacies.push_back({ BasePosition + glm::vec3(1,0,offset) });
		Mesh->Vertacies.push_back({ BasePosition + glm::vec3(1,1,offset) });
		Mesh->Vertacies.push_back({ BasePosition + glm::vec3(0,1,offset) });

		AddQuad(offset == 1);
	}

	SetNormals({ ox, oy, oz });
}

void Chunk::AddQuad(bool Reverse)
{
	int top = Mesh->Vertacies.size() - 1;

	for (int i = 0; i < 4; ++i)
	{
		int ri;

		if (Reverse)
			ri = 3 - i;
		else
			ri = i;

		Mesh->Indicies.push_back(top - ri);
	}
}

void Chunk::SetNormals(glm::vec3 Direction)
{
	int top = Mesh->Vertacies.size() - 1;

	for (int i = 0; i < 4; ++i)
	{
		Mesh->Vertacies[top - i].iNormal = Direction;
	}
}

Block Chunk::GetBlock(int x, int y, int z)
{
	if (x < 0)
	{
		return world->GetChunk(_PositionX - CHUNKWIDTH, _PositionZ)->GetBlock(CHUNKWIDTH - 1,y,z);
	}

	if (x >= CHUNKWIDTH)
	{
		return world->GetChunk(_PositionX + CHUNKWIDTH, _PositionZ)->GetBlock(0, y, z);
	}

	if (y < 0 || y >= CHUNKHEIGHT)
		return { 1 };

	if (z < 0)
	{
		return world->GetChunk(_PositionX , _PositionZ - CHUNKWIDTH)->GetBlock(x, y, CHUNKWIDTH - 1);
	}

	if (z >= CHUNKWIDTH)
	{
		return world->GetChunk(_PositionX, _PositionZ + CHUNKWIDTH)->GetBlock(x, y, 0);
	}

	return BlockBuffer[BLOCKINDEX];
}

bool Chunk::ReadyForRendering()
{
	return MeshGenerated && DataGenerated;
}

void Chunk::Render()
{
	if (ReadyForRendering())
	{
		Mesh->Draw();
	}
	else
	{
		world->InsertChunkToCreationQue(this);
	}
}

void Chunk::TestGenerate()
{
	Mesh->Vertacies.push_back({ {0,0,0} });
	Mesh->Vertacies.push_back({ {1,0,0} });
	Mesh->Vertacies.push_back({ {1,1,0} });

	Mesh->Indicies.push_back(0);
	Mesh->Indicies.push_back(1);
	Mesh->Indicies.push_back(2);

	Mesh->RenderMode = GL_TRIANGLES;

	DataGenerated = true;
	MeshGenerated = true;
}