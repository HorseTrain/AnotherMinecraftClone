#include "RenderTexture.h"
#include "..\ImageLoader.h"
#include "..\..\OpenGL/glew/include/GL/glew.h"

using namespace Game::Graphics;

RenderTexture::RenderTexture()
{

}

RenderTexture::~RenderTexture()
{
	if (TextureBuffer == nullptr)
	{
		delete TextureBuffer;
	}
}

void RenderTexture::LoadFromFile(std::string path)
{
	if (TextureBuffer == nullptr)
	{
		free(TextureBuffer);
	}

	int cbi;

	TextureBuffer = (char*)stbi_load(path.c_str(),&Width, &Height, &cbi, 4);

	glGenTextures(1,(GLuint*)&Handle);
	glBindTexture(GL_TEXTURE_2D, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

void RenderTexture::Use()
{
	glBindTexture(GL_TEXTURE_2D, Handle);

	glActiveTexture(Handle);
}