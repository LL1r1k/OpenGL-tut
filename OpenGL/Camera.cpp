#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 worldUp)
{
	ViewMatrix = glm::mat4(1.f);
	movementSpeed = 3.f;
	sensitivity = 5.f;

	this->worldUp = worldUp;
	this->position = position;
	this->right = glm::vec3(0.f);
	this->up = worldUp;

	pitch = 0.f;
	yaw = 0.f;
	roll = 0.f;

	updateCameraVectors();
}

Camera::~Camera()
{

}

const glm::mat4 Camera::getViewMatrix()
{
	updateCameraVectors();

	ViewMatrix = glm::lookAt(position, position + front, up);

	return ViewMatrix;
}

const glm::vec3 Camera::getPosition() const
{
	return position;
}

void Camera::updateInput(const float & dt, const int direction, const double & offsetX, const double & offsetY)
{
	updateMouseInput(dt, direction, offsetX, offsetY);	
}

void Camera::updateMouseInput(const float & dt, const int direction, const double & offsetX, const double & offsetY)
{
	pitch += static_cast<double >(offsetY) * dt * sensitivity;
	yaw += static_cast<double>(offsetX) * dt * sensitivity;

	if (pitch >= 80.f)
		pitch = 80.f;
	else if (pitch <= -80.f)
		pitch = -80.f;

	if (yaw > 360.f || yaw < -360.f)
		yaw = 0.f;
}

void Camera::move(const float & dt, const int & direction)
{
	switch (direction)
	{
	case FORWARD:
		position += front * dt * movementSpeed;
		break;
	case BACK:
		position -= front * dt * movementSpeed;
		break;
	case LEFT:
		position -= right * dt * movementSpeed;
		break;
	case RIGHT:
		position += right * dt * movementSpeed;
		break;
	default:
		break;
	}
}

void Camera::updateCameraVectors()
{
	front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	front.y = glm::sin(glm::radians(pitch));
	front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
