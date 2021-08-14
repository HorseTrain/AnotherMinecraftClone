#pragma once

#include <inttypes.h>
#include <string>
#include <vector>
#include <map>
#include "..\glm\glm\mat4x4.hpp"

namespace Game
{
	namespace Graphics
	{
		class ShaderSource
		{
		public:

			uint32_t Type;
			std::string Source;

			//Never call this outside of RenderShader::CompileSources, or you WILL get leaks.
			int Compile();

			ShaderSource();
			ShaderSource(std::string Source, uint32_t Type);
			~ShaderSource();
		};

		class RenderShader
		{
		private:

			void Delete();

			std::map<std::string, int> Handles;

		public:

			uint32_t ProgramHandle = UINT_MAX;

			std::vector<ShaderSource> Sources;

			void CompileSources();
			void Use();

			int GetUniformLocation(std::string name);

			void UniformMat4(std::string name,glm::mat4 data);

			RenderShader();
			~RenderShader();
		};

	}
}