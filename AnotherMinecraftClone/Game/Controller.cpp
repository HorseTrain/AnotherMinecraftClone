#include "Controller.h"

#include "..\OpenGL/glfw/include/GLFW/glfw3.h"

#include <iostream>
#include "glm/glm/mat4x4.hpp"
#include "World.h"

using namespace Game;

KeyboardController::KeyboardController(GLFWwindow* window)
{
	this->window = window;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void KeyboardController::Update()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		Close = true;
	}

	double mx;
	double my;

	glfwGetCursorPos(window, &mx, &my );

	glm::vec2 CurrentMouse = glm::vec2(mx, my);

	glm::vec2 MouseVelocity = CurrentMouse - MousePosition;

	MousePosition = glm::vec2(mx, my);

	CameraStick = glm::vec2(MouseVelocity.y, MouseVelocity.x) * 0.1f;

	float VelF = glfwGetKey(window, GLFW_KEY_W) - glfwGetKey(window, GLFW_KEY_S);
	float VelR = glfwGetKey(window, GLFW_KEY_D) - glfwGetKey(window, GLFW_KEY_A);

	Velocity = glm::vec3(VelR,0, -VelF) * 2.0f;
}