#pragma once
#include <string>

namespace Game
{
	char* ReadFile(std::string path, int* size);
	std::string ReadFileText(std::string path);

	std::string GetAssetPath(std::string path);
}