#include "RenderShader.h"
#include "..\..\OpenGL/glew/include/GL/glew.h"

using namespace Game::Graphics;

RenderShader::RenderShader()
{

}

void RenderShader::CompileSources()
{
	Delete();

	ProgramHandle = glCreateProgram();

	for (ShaderSource source : Sources)
	{
		int ShaderHandle = source.Compile();

		glAttachShader(ProgramHandle, ShaderHandle);

		glDeleteShader(ShaderHandle);
	}

	glLinkProgram(ProgramHandle);
	glValidateProgram(ProgramHandle);
}

void RenderShader::Use()
{
	if (ProgramHandle != UINT_MAX)
		glUseProgram(ProgramHandle);
}

int RenderShader::GetUniformLocation(std::string name)
{
	Use();

	if (Handles.find(name) == Handles.end())
	{
		Handles[name] = glGetUniformLocation(ProgramHandle, name.c_str());
	}

	return Handles[name];
}

RenderShader::~RenderShader()
{
	Delete();
}

void RenderShader::Delete()
{
	Handles = std::map<std::string, int>();

	if (ProgramHandle != UINT_MAX)
	{
		glDeleteProgram(ProgramHandle);
	}

	ProgramHandle = UINT_MAX;
}

void RenderShader::UniformMat4(std::string name, glm::mat4 data)
{
	int handle = GetUniformLocation(name);

	glUniformMatrix4fv(handle, 1, false, (GLfloat*)&data);
}