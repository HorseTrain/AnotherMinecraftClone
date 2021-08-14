#pragma once

#include "Exporter.h"
#include <inttypes.h>

namespace Tools
{
	struct TextureAtlasHeader
	{
		uint32_t TextureWidth;
		uint32_t TextureHeight;
		uint16_t EntryCount;
	};

	struct Face
	{
		uint16_t iX;
		uint16_t iY;
	};

	struct TextureAtlasEntry
	{
		uint16_t BlockID;

		Face PX;
		Face NX;
		Face PY;
		Face NY;
		Face PZ;
		Face NZ;
	};
}