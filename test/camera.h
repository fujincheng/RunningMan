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
		cameraPos += distance * cameraLeft;
	}
	void moveRight(GLfloat distance) {
		cameraPos -= distance * cameraLeft;
	}
	void moveForward(GLfloat distance) {
		cameraPos += distance * cameraFront;
	}
	void moveBackward(GLfloat distance) {
		cameraPos -= distance * cameraFront;
	}
	void moveXTo(GLfloat x) {
		cameraPos = glm::vec3(x, cameraPos.y, cameraPos.z);
	}
	const float* getMat() {
		return glm::value_ptr(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
	}
private:
	glm::vec3 cameraPos;
	static const glm::vec3 cameraFront;
	static const glm::vec3 cameraUp;
	static const glm::vec3 cameraLeft;
};

const glm::vec3 Camera::cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 Camera::cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 Camera::cameraLeft = glm::vec3(-1.0f, 0.0f, 0.0f);
const GLfloat Camera::cameraSpeed = 0.1f;

#endif // !CAMERA_H