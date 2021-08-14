#pragma once

#include "World.h"

using namespace Game::MathHelpers;

float Game::MathHelpers::DegToRad(float deg)
{
	return deg / (180 / M_PI);
}

//https://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/index.htm
glm::quat Game::MathHelpers::GetAxisAngle(glm::vec3 Direction, float Angle)
{
	float qx = Direction.x * sin(Angle / 2);
	float qy = Direction.y * sin(Angle / 2);
	float qz = Direction.z * sin(Angle / 2);
	float qw = cos(Angle / 2);
	
	return glm::quat(qw, qx, qy, qz);
}

glm::mat4 Game::MathHelpers::Translate(glm::vec3 Translation)
{
	return glm::mat4(
	
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		Translation.x,Translation.y,Translation.z,1
	
	);
}

glm::mat4 Game::MathHelpers::Rotate(glm::quat Rotation)
{
	return glm::toMat4(Rotation);
}

glm::vec3 Game::MathHelpers::GetForward(glm::mat4 Transform)
{
	return { Transform[0][2],Transform[1][2],Transform[2][2] };
}

glm::vec3 Game::MathHelpers::GetRight(glm::mat4 Transform)
{
	return { Transform[0][0],Transform[1][0],Transform[2][0] };
}