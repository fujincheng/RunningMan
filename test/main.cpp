#include <iostream>

#define GLFW_INCLUDE_GLU

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model.h"
#include "texture.h"
#include "camera.h"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
const GLuint WIDTH = 500, HEIGHT = 500;
bool keys[1024];
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

void do_movement()
{
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
}

void drawGun(Model gun) {
	static float angle = 0;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	angle += 0.5;
	glRotatef(angle, 0.f, 1.f, 0.f);
	glScalef(0.01f, 0.01f, 0.01f);
	gun.draw();
}

void drawRoad(GLuint texture) {	
	glBindTexture(GL_TEXTURE_2D, texture);
	glPushMatrix();
	glMultMatrixf(camera.getMat());
	glTranslatef(0.0f, 0.0f, -3.0f);
	//glRotatef(45, 1, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(-0.5, -0.5);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(+0.5, -0.5);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(+0.5, +0.5);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(-0.5, +0.5);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}

int main()
{

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Running", nullptr, nullptr);
	if (window == nullptr)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	
	glfwSetKeyCallback(window, key_callback);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	
	//glEnable(GL_LIGHTING);
	GLfloat ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat position[] = { 0.0, 1.0, 0.0, 0.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	
	glEnable(GL_TEXTURE_2D);

	
	Texture road("imgs/road.jpg");
	Model gun("models/rifle.obj");
	
	glFrustum(-5, 5, -5, 5, 0.5, 20.5);
	camera.moveUp(3.0f);
	while (!glfwWindowShouldClose(window))
	{	
		do_movement();

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// drawGun(gun);
		
		//drawRoad(road.texture);

		glPushMatrix();
		
		glMultMatrixf(camera.getMat());

		glPushMatrix();
		glTranslatef(-8, 0, -2);
		for (int i = 0; i < 5; ++i) {
			drawACube();
			glTranslatef(0, 0, -4);
		}
		glPopMatrix();

		glPushMatrix();
		glTranslatef(8, 0, -2);
		for (int i = 0; i < 5; ++i) {
			drawACube();
			glTranslatef(0, 0, -4);
		}
		glPopMatrix();

		glPopMatrix();
		
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

