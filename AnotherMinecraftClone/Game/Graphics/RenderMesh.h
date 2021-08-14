#pragma once

#include <vector>
#include "..\glm\glm\vec2.hpp"
#include "..\glm\glm\vec3.hpp"
#include <inttypes.h>

namespace Game
{
	namespace Graphics
	{
		struct Vertex
		{
			glm::vec3 iPosition;
			glm::vec3 iNormal;
			glm::vec2 iUV;
		};

		class RenderMesh
		{
		private:

			uint32_t vao;
			uint32_t vbo;
			uint32_t ibo;

			bool IsUploaded = false;

		public:

			uint32_t RenderMode;

			std::vector<uint32_t> Indicies;
			std::vector<Vertex> Vertacies;

			void Upload();
			void Draw();

			RenderMesh();
			~RenderMesh();
		};
	}
}