#include "ChunkDataGenerator.h"
#include <random>

#include "glm/glm/vec3.hpp"
#include "glm/glm/vec2.hpp"

#include <iostream>
#include "World.h"

using namespace Game;

int Noise::Seed = 0;

uint32_t hash(uint32_t x) 
{
    x += (x << 10u);
    x ^= (x >> 6u);
    x += (x << 3u);
    x ^= (x >> 11u);
    x += (x << 15u);
    return x;
}

int Noise::GetHeightRaw(int x, int z)
{
    srand(hash((x * z) + x + z));  
    
    return rand() & UCHAR_MAX;
}

float lerp(float first, float second, float l)
{
	return first + (l * (second - first));
}

float Noise::gh(float x, float z)
{
    int q0 = GetHeightRaw(x, z);
    int q1 = GetHeightRaw(x + 1, z);

    float l0 = x - (int)x;

    return lerp(q0, q1, l0) / 255.0f;
}

float Noise::GetHeight(float x, float z)
{
    return (sin(x) + sin(z)) * (sin(x)*10);
}