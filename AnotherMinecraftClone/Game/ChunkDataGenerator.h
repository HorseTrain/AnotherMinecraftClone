#pragma once

namespace Game
{
	class Noise
	{
	private:

		int GetHeightRaw(int x, int z);

	public:
		
		static int Seed;

		float gh(float x, float z);
		float GetHeight(float x, float z);
	};

	class ChunkDataGenerator
	{
	public:

		Noise noise;

		int xLast;
		int zLast;
		int LastHeight;

		int GetBlockID(int x, int y, int z);
	};
}