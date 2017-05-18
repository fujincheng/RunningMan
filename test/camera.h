#ifndef CAMERA_H
#define CAMERA_H
#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Camera {
public:
	static const GLfloat cameraSpeed;
	Camera() {
		this->cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	void moveUp(GLfloat distance) {
		cameraPos += distance * cameraUp;
	}
	void moveDown(GLfloat distance) {
		cameraPos -= distance * cameraUp;
	}
	void moveLeft(GLfloat distance) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * distance;
	}
	void moveRight(GLfloat distance) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * distance;
	}
	void moveForward(GLfloat distance) {
		cameraPos += distance * cameraFront;
	}
	void moveBackward(GLfloat distance) {
		cameraPos -= distance * cameraFront;
	}
	const float* getMat() {
		return glm::value_ptr(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
	}
private:
	glm::vec3 cameraPos;
	static const glm::vec3 cameraFront;
	static const glm::vec3 cameraUp;
};

const glm::vec3 Camera::cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 Camera::cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
const GLfloat Camera::cameraSpeed = 0.02f;

#endif // !CAMERA_H