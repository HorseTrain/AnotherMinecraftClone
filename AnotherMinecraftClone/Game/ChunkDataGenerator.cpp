#include "ChunkDataGenerator.h"
#include <iostream>

using namespace Game;

int ChunkDataGenerator::GetBlockID(int x, int y, int z)
{
	float scale = 100;

	if (!(x == xLast && z == zLast))
	{
		xLast = x;
		zLast = z;

		LastHeight = sin((float)x / scale) * 20;
	}

	return y < LastHeight ? 1 : 0;
}