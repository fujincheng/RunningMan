#include <iostream>
#include <Windows.h>

#define GLFW_INCLUDE_GLU

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model.h"
#include "texture.h"
#include "camera.h"
#include "skybox.h"

void initGL();
void do_movement();
void drawString(const char* str);
void drawRoad(Texture & road);
void drawTrees(Model & tree);
void drawPlayer(Model & model);
void displayText();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLuint WIDTH = 500, HEIGHT = 500;
bool keys[1024];
float playerOffsetZ = 1.0f, playerOffsetX = 0.0f, playerRotated = 180.0f;
Camera camera;
const int MAX_CHAR = 128;
GLuint drawRoadList;

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

	initGL();
	
	Texture road("imgs/road.jpg");
	Model player("models/pig/pig.obj");
	Model tree("models/Palm_Tree/palm_Tree.obj");
	Skybox skybox;
	
	gluPerspective(75, GLfloat(WIDTH) / HEIGHT, 1, 128);
	camera.moveUp(5.0f);
	camera.moveBackward(9.0f);
	
	while (!glfwWindowShouldClose(window)) {	
		do_movement();
		
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glPushMatrix();
		glMultMatrixf(camera.getMat());
		skybox.draw(0, 0, 0, 50, 50, 100);
		drawRoad(road);
		drawTrees(tree);
		drawPlayer(player);
		displayText();
		glPopMatrix();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteLists(drawRoadList, 1);
	glfwTerminate();
	return 0;
}

void initGL() {
	GLfloat lightAmbient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lightPosition[] = { 0.0, 0.0, 0.0, 0.0 };
	GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
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
	if (keys[GLFW_KEY_J])
		playerOffsetX -= Camera::cameraSpeed;
	if (keys[GLFW_KEY_L])
		playerOffsetX += Camera::cameraSpeed;
	if (keys[GLFW_KEY_I])
		playerOffsetZ += Camera::cameraSpeed;
	if (keys[GLFW_KEY_K])
		playerOffsetZ -= Camera::cameraSpeed;
	if (keys[GLFW_KEY_U])
		playerRotated += 5.0f;
	if (keys[GLFW_KEY_O])
		playerRotated -= 5.0f;
}

void drawString(const char* str) {
	static bool isFirstCall = true;
	static GLuint lists;

	if (isFirstCall) {
		isFirstCall = false;
		lists = glGenLists(MAX_CHAR);
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}

	int len = strlen(str);
	for (int i = 0; i < len; ++i)
		glCallList(lists + str[i]);
}

void drawRoad(Texture & road) {
	static bool isFirstCall = true;
	GLuint texture = road.texture;
	glEnable(GL_LIGHTING);
	if (isFirstCall) {
		isFirstCall = false;
		drawRoadList = glGenLists(1);
		glNewList(drawRoadList, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, texture);
		glPushMatrix();
		glTranslatef(0.0f, -1.0f, 1.0f);
		glRotatef(90, 1, 0, 0);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 0.0);
		glVertex2d(-7, -30);
		glTexCoord2d(7.0, 0.0);
		glVertex2d(7, -30);
		glTexCoord2d(7.0, 30.0);
		glVertex2d(7, 0);
		glTexCoord2d(0.0, 30.0);
		glVertex2d(-7, 0);
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
		glEndList();
	}
	glCallList(drawRoadList);
}

void drawTrees(Model & tree) {
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(-6.5f, -1.0f, 0.0f);
	for (int i = 0; i < 7; ++i) {
		glPushMatrix();
		tree.draw(3);
		glPopMatrix();
		glTranslatef(0, 0, -4.0f);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(6.5f, -1.0f, 0.0f);
	for (int i = 0; i < 7; ++i) {
		glPushMatrix();
		glRotatef(180, 0, 1, 0);
		tree.draw(3);
		glPopMatrix();
		glTranslatef(0, 0, -4.0f);
	}
	glPopMatrix();
}

void drawPlayer(Model & model) {
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(playerOffsetX, -1.0f, -playerOffsetZ);
	glRotatef(playerRotated, 0, 1, 0);
	model.draw(1.8f);
	glPopMatrix();
}

void displayText() {
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glTranslatef(playerOffsetX, 0.0f, -playerOffsetZ);
	glRasterPos2f(-0.4f, 1.0f);
	drawString("You");
	glPopMatrix();
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