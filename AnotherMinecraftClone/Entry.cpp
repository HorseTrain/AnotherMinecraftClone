#include "OpenGL/glew/include/GL/glew.h"
#include "OpenGL/glfw/include/GLFW/glfw3.h"

#include "Game/Graphics/RenderMesh.h"
#include "Game/Graphics/RenderShader.h"
#include "Game/World.h"
#include <chrono>

#include <iostream>

using namespace Game::Graphics;
using namespace Game;

#include "Game/ChunkDataGenerator.h"
#include <fstream>

void TestNoise()
{
	Game::ChunkDataGenerator test = Game::ChunkDataGenerator();

	std::ofstream out = std::ofstream();

	out.open("E:\\Projects\\Build\\img.ppm");

	int w = 1920;
	int h = 1080;

	out << "P3" << std::endl;
	out << w << " " << h << std::endl;
	out << "255" << std::endl;

	float scale = 200.0f;

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int col = 255;

			float height = test.noise.GetHeight(x, 0) * 256;

			if (y > height)
				col = 0;

			out << col << " " << col << " " << col << std::endl;
		}
	}
}

#include "Game/ImageLoader.h"
#include "Tools/TextureDataGenerator.h"
#include "Tools/ListTools.h"

void CreateTextureAtlas()
{
	std::vector<char> out;

	std::vector<Tools::TextureAtlasEntry> Entries;

							//ID	PX		NX		PY		NY		PZ		NZ
	Entries.push_back({		0,		{3,0},	{3,0},	{0,0},	{2,0},	{3,0},	{3,0} });

	Tools::TextureAtlasHeader header = { 256, 256, Entries.size() };

	Tools::WriteStruct(&out, header);

	for (Tools::TextureAtlasEntry entry : Entries)
	{
		Tools::WriteStruct(&out, entry);
	}

	std::ofstream file = std::ofstream();

	file.open("E:/Projects/AnotherMinecraftClone/Assets/ta", std::ios::binary);

	for (char c : out)
	{
		file << c << std::endl;
	}

	file.close();
}

int main()
{
	//CreateTextureAtlas();

	//return -1;

	//TestNoise();

	//return -1;

	glfwInit();

	//glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
	GLFWwindow* mainWindow = glfwCreateWindow(1000, 1000,"Another Minecraft Clone",NULL, NULL);

	glfwMakeContextCurrent(mainWindow);

	glewInit();

	World world;

	world.WorldWindow = mainWindow;

	world.Run();

	glfwTerminate();
}