#pragma once

#include "glm/glm/vec3.hpp"
#include "glm/glm/vec2.hpp"

struct GLFWwindow;

namespace Game
{
	class Controller
	{
	public:

		bool Close;

		glm::vec3 Velocity;
		glm::vec2 CameraStick;

		virtual void Update() { throw 0; };
	};

	class KeyboardController : public Controller
	{
	private:

		GLFWwindow* window;

		glm::vec2 MousePosition;

	public:

		KeyboardController(GLFWwindow* window);

		void Update() override;
	};
}