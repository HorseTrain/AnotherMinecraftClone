#include "Core.h"
#include <iostream>
#include <fstream>

using namespace Game;

char* Game::ReadFile(std::string path, int* size)
{
	std::ifstream stream = std::ifstream();

	stream.open(path, std::ios::binary);

	stream.seekg(0,std::ios::end);

	*size = stream.tellg();

	stream.seekg(0, std::ios::beg);

	char* out = new char[*size];

	stream.read(out, *size);

	return out;
}

std::string Game::ReadFileText(std::string path)
{
	int size;

	char* Buffer = ReadFile(path, &size);

	std::string out = "";

	for (int i = 0; i < size; ++i)
	{
		out += Buffer[i];
	}

	return out;
}


std::string Game::GetAssetPath(std::string path)
{
	return "E:/Projects/AnotherMinecraftClone/Assets/" + path;
}