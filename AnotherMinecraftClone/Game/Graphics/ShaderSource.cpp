#include <iostream>
#include "RenderShader.h"
#include "..\..\OpenGL/glew/include/GL/glew.h"

using namespace Game::Graphics;

int ShaderSource::Compile()
{
	uint32_t Out = glCreateShader(Type);

	const char* source = Source.c_str();
	int Size = Source.size();

	glShaderSource(Out, 1, &source, &Size);
	glCompileShader(Out);

	int successfull;
	glGetShaderiv(Out, GL_COMPILE_STATUS, &successfull);

	if (!successfull)
	{
		Size = 1000;

		char Error[1000];

		glGetShaderInfoLog(Out, Size,&Size, (GLchar*)&Error);

		std::cout << Error;

		throw 0;
	}

	return Out;
}

ShaderSource::ShaderSource(std::string Source, uint32_t Type)
{
	this->Source = Source;
	this->Type = Type;
}

ShaderSource::ShaderSource()
{

}

ShaderSource::~ShaderSource()
{

}