#pragma once

#include <vector>

namespace Tools
{
	template <typename T>
	void WriteStruct(std::vector<char>* out, T data)
	{
		for (int i = 0; i < sizeof T; ++i)
		{
			out->push_back(*((char*)&data + i));
		}
	}
}