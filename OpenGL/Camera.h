#pragma once

#include <iostream>

#include <glew.h> 
#include <glfw3.h> 

#include <glm.hpp> 
#include <vec3.hpp>
#include <gtc\matrix_transform.hpp>
#include <mat4x4.hpp>

enum direction
{
	FORWARD = 0, BACK = 1, LEFT = 2, RIGHT = 3
};

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 worldUp);
	~Camera();

	const glm::mat4 getViewMatrix();
	const glm::vec3 getPosition() const;

	void updateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY);
	void updateMouseInput(const float& dt, const int direction, const double& offsetX, const double& offsetY);
	void move(const float& dt, const int& direction);

private:
	glm::mat4 ViewMatrix;
	glm::vec3 position, front, right, up, worldUp;

	GLfloat pitch, yaw, roll;
	GLfloat movementSpeed, sensitivity;

	void updateCameraVectors();

};

