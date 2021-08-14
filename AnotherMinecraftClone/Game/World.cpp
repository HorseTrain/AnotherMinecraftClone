#include "World.h"
#include <iostream>
#include <string>
#include <thread>

#include "Controller.h"

#include "..\OpenGL/glew/include/GL/glew.h"
#include "..\OpenGL/glfw/include/GLFW/glfw3.h"

#include "Graphics/RenderShader.h"
#include "Graphics/RenderTexture.h"

using namespace Game;

float World::DeltaTime = 0;

World* World::MainWorld;

void World::Run()
{
	glfwMakeContextCurrent(WorldWindow);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	MainController = new KeyboardController(WorldWindow);

	CreateChunk(0,0);

	MainWorld = this;

	Graphics::RenderTexture texture;

	Chunk::LoadChunkShader();

	texture.LoadFromFile("E:/Projects/AnotherMinecraftClone/Assets/terrain.png");
	texture.Use();

	while (!MainController->Close)
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1, 0.1, 0.1,1);

		UpdateWorld();

		glfwSwapBuffers(WorldWindow);

		glfwGetWindowSize(WorldWindow, &WindowWidth, &WindowHeight);
		glViewport(0, 0, WindowWidth, WindowHeight);

		SubmitFrameTime();
	}
}

void World::SubmitFrameTime()
{
	glfwSetWindowTitle(WorldWindow, ("Another Minecraft Clone: " + std::to_string(FrameRate) + " fps").c_str());

	FrameRate = 1000000000.0 / (double)std::chrono::duration_cast<std::chrono::nanoseconds>(FrameCounter.now() - Last).count();

	Last = FrameCounter.now();

	if (FrameRate < 10)
		FrameRate = 10;

	DeltaTime = 60 / FrameRate;
}

void World::UpdateWorld()
{
	MainController->Update();

	MainCamera.Update(MainController, DeltaTime);

	GenerateAllChunks();
	RenderAllChunks();
}

uint64_t World::GetKey(uint64_t X, uint64_t Z)
{
	return ((X) | (Z << 32));
}

bool World::ChunkExists(uint32_t X, uint32_t Z)
{
	return ChunkMap.find(GetKey(X, Z)) != ChunkMap.end();
}

Chunk* World::GetChunk(uint32_t X, uint32_t Z)
{
	if (ChunkExists(X, Z))
	{
		return ChunkMap[GetKey(X, Z)];
	}

	return nullptr;
}

void World::CreateChunk(uint32_t X, uint32_t Z)
{
	if (ChunkExists(X, Z))
		return;

	glm::vec3 CameraPos = MainCamera.Position;
	CameraPos.y = 0;

	glm::vec3 MyPos = glm::vec3((int)X, 0 , (int)Z);

	float dis = glm::length(CameraPos - MyPos);

	if (dis > RENDERDISTANCE)
		return;

	uint64_t Key = GetKey(X, Z);

	Chunk* chunk = new Chunk(X, Z, this);

	chunk->Key = GetKey(X, Z);

	InsertChunkToCreationQue(chunk);

	ChunkMap[Key] = chunk;
}

bool World::TestForChunkDestruction(Chunk* chunk)
{
	glm::vec3 CameraPos = MainCamera.Position;
	CameraPos.y = 0;

	glm::vec3 MyPos = glm::vec3((int)chunk->_PositionX, 0, (int)chunk->_PositionZ);

	float dis = glm::length(CameraPos - MyPos);

	return dis > RENDERDISTANCE;
}

void World::InsertChunkToCreationQue(Chunk* chunk)
{
	ChunkCreationQue.push_back(chunk);
}

void World::GenerateAllChunks()
{
	for (Chunk* chunk : ChunkCreationQue)
	{
		chunk->GenerateChunk();
	}

	ChunkCreationQue = std::vector<Chunk*>();

	for (Chunk* chunk : ChunkDestructionQue)
	{
		ChunkMap.erase(chunk->Key);

		delete chunk;
	}

	ChunkDestructionQue = std::vector<Chunk*>();
}

void World::RenderAllChunks()
{
	Chunk::LoadChunkShader();
	Chunk::ChunkShader->Use();

	Chunk::ChunkShader->UniformMat4("CameraView", MainCamera.GetViewMatrix());

	for (auto iterator = ChunkMap.begin(); iterator != ChunkMap.end(); iterator++)
	{
		Chunk* CurrentChunk = iterator->second;

		if (TestForChunkDestruction(CurrentChunk))
		{
			ChunkDestructionQue.push_back(CurrentChunk);

			continue;
		}

		CreateChunk(CurrentChunk->_PositionX + CHUNKWIDTH, CurrentChunk->_PositionZ);
		CreateChunk(CurrentChunk->_PositionX - CHUNKWIDTH, CurrentChunk->_PositionZ);
		CreateChunk(CurrentChunk->_PositionX, CurrentChunk->_PositionZ + CHUNKWIDTH);
		CreateChunk(CurrentChunk->_PositionX, CurrentChunk->_PositionZ - CHUNKWIDTH);

		CurrentChunk->Render();
	}
}

World::~World()
{
	delete MainController;
}

World::World()
{

}