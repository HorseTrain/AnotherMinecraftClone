#pragma once
#include <string>

namespace Game
{
	namespace Graphics
	{
		class RenderTexture
		{
		private:

			char* TextureBuffer;

		public:
			int Handle;

			int Width;
			int Height;

			void LoadFromFile(std::string path);
			void Use();

			RenderTexture();
			~RenderTexture();
		};
	}
}