#include "World.h"

#include "Controller.h"

using namespace Game;
using namespace glm;

glm::mat4 Camera::GetRotationMatrix()
{
	quat rx = MathHelpers::GetAxisAngle({ 1,0,0 }, MathHelpers::DegToRad(EularAngleRotation.x));
	quat ry = MathHelpers::GetAxisAngle({ 0,1,0 }, MathHelpers::DegToRad(EularAngleRotation.y));

	return MathHelpers::Rotate(rx * ry);
}

glm::mat4 Camera::GetTranslationMatrix()
{
	return MathHelpers::Translate(-Position);
}

mat4 Camera::GetTransformMatrix()
{
	return GetRotationMatrix() * GetTranslationMatrix();
}

mat4 Camera::GetViewMatrix()
{
	glm::mat4 view = glm::perspective(MathHelpers::DegToRad(fov), 1.0f,1.0f,2000.0f);

	return view * GetTransformMatrix();
}

void Camera::Update(Controller* controller, float DeltaTime)
{
	EularAngleRotation += glm::vec3(controller->CameraStick.x, controller->CameraStick.y, 0) * DeltaTime;

	if (EularAngleRotation.x > 80)
		EularAngleRotation.x = 80;

	if (EularAngleRotation.x < -80)
		EularAngleRotation.x = -80;

	glm::mat4 Rot = GetRotationMatrix();

	Forward = MathHelpers::GetForward(Rot);

	Position += (Forward * controller->Velocity.z) + (MathHelpers::GetRight(Rot) * controller->Velocity.x)* DeltaTime;
} 