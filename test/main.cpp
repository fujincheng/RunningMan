#include <iostream>

#define GLFW_INCLUDE_GLU

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model.h"
#include "texture.h"
#include "camera.h"
#include "skybox.h"

enum playerPosition {left, right};
playerPosition position = playerPosition::left;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
const GLuint WIDTH = 500, HEIGHT = 500;
bool keys[1024];
float playerDistance = 0.0f;
Camera camera;

const double vertexs[9][3] = {
	{ 0, 0, 0 },
	{ -1, 1, 1 },
	{ -1, -1, 1 },
	{ 1, 1, 1 },
	{ 1, -1, 1 },
	{ -1, 1, -1 },
	{ -1, -1, -1 },
	{ 1, 1, -1 },
	{ 1, -1, -1 },
};

void drawACube() {
	glBegin(GL_QUADS);

	glColor3d(0, 0, 1);
	glVertex3dv(vertexs[1]);
	glVertex3dv(vertexs[2]);
	glVertex3dv(vertexs[4]);
	glVertex3dv(vertexs[3]);

	glColor3d(0, 1, 0);
	glVertex3dv(vertexs[3]);
	glVertex3dv(vertexs[4]);
	glVertex3dv(vertexs[8]);
	glVertex3dv(vertexs[7]);

	glColor3d(0, 1, 1);
	glVertex3dv(vertexs[7]);
	glVertex3dv(vertexs[8]);
	glVertex3dv(vertexs[6]);
	glVertex3dv(vertexs[5]);

	glColor3d(1, 0, 0);
	glVertex3dv(vertexs[5]);
	glVertex3dv(vertexs[6]);
	glVertex3dv(vertexs[2]);
	glVertex3dv(vertexs[1]);

	glColor3d(1, 0, 1);
	glVertex3dv(vertexs[5]);
	glVertex3dv(vertexs[1]);
	glVertex3dv(vertexs[3]);
	glVertex3dv(vertexs[7]);

	glColor3d(1, 1, 0);
	glVertex3dv(vertexs[6]);
	glVertex3dv(vertexs[2]);
	glVertex3dv(vertexs[4]);
	glVertex3dv(vertexs[8]);

	glEnd();
}

void do_movement() {
	if (keys[GLFW_KEY_W])
		camera.moveUp(Camera::cameraSpeed);
	if (keys[GLFW_KEY_S])
		camera.moveDown(Camera::cameraSpeed);
	if (keys[GLFW_KEY_A])
		camera.moveLeft(Camera::cameraSpeed);
	if (keys[GLFW_KEY_D])
		camera.moveRight(Camera::cameraSpeed);
	if (keys[GLFW_KEY_Q])
		camera.moveForward(Camera::cameraSpeed);
	if (keys[GLFW_KEY_E])
		camera.moveBackward(Camera::cameraSpeed);
	if (keys[GLFW_KEY_J]) {
		position = playerPosition::left;
		camera.moveXTo(-3.5f);
	}
	if (keys[GLFW_KEY_K]) {
		position = playerPosition::right;
		camera.moveXTo(3.5f);
	}
	if (keys[GLFW_KEY_U])
		playerDistance -= Camera::cameraSpeed;
	if (keys[GLFW_KEY_I])
		playerDistance += Camera::cameraSpeed;
}

void drawRoad(GLuint texture) {	
	glBindTexture(GL_TEXTURE_2D, texture);
	glPushMatrix();
	glTranslatef(0.0f, -1.0f, 1.0f);
	glRotatef(90, 1, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(-7, -20);
	glTexCoord2d(7.0, 0.0);
	glVertex2d(7, -20);
	glTexCoord2d(7.0, 20.0);
	glVertex2d(7, 0);
	glTexCoord2d(0.0, 20.0);
	glVertex2d(-7, 0);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void drawCubes() {
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(-8, 0, 0);
	for (int i = 0; i < 5; ++i) {
		drawACube();
		glTranslatef(0, 0, -3);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8, 0, 0);
	for (int i = 0; i < 5; ++i) {
		drawACube();
		glTranslatef(0, 0, -3);
	}
	glPopMatrix();
}


void drawModel(Model model) {
	glEnable(GL_LIGHTING);
	glPushMatrix();
	if (position == playerPosition::left) 
		glTranslatef(-3.5f, -0.2f, 0.1f - playerDistance);
	else
		glTranslatef(3.5f, -0.2f, 0.1f - playerDistance);
	glScalef(0.1f, 0.1f, 0.1f);
	model.draw();
	glPopMatrix();
}


int main() {

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Running", nullptr, nullptr);
	if (window == nullptr) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	
	glfwSetKeyCallback(window, key_callback);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	GLfloat lightAmbient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lightPosition[] = { 0.0, 1.0, 0.0, 0.0 };
	GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	
	glEnable(GL_TEXTURE_2D);

	
	Texture road("imgs/road.jpg");
	Model gun("models/X1.obj");
	Skybox skybox;
	
	gluPerspective(75, GLfloat(WIDTH) / HEIGHT, 1, 128);
	camera.moveUp(5.0f);
	camera.moveLeft(3.5f);
	camera.moveBackward(8.5f);
	
	while (!glfwWindowShouldClose(window)) {	
		do_movement();
		
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glPushMatrix();
		glMultMatrixf(camera.getMat());
		skybox.draw(0, 0, 0, 40, 40, 100);
		drawRoad(road.texture);
		drawCubes();
		drawModel(gun);
		glPopMatrix();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	gun.destroy();
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

