#ifndef TEXTURE_H
#define TEXTURE_H
#pragma once

#include <soil.h>
#include <GLFW/glfw3.h>
using namespace std;

class Texture {
public:
	GLuint texture;
	Texture(const char* path) {
		this->data = SOIL_load_image(path, &this->width, &this->height, 0, SOIL_LOAD_RGB);
		glGenTextures(1, &this->texture);
		glBindTexture(GL_TEXTURE_2D, this->texture);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->data);
	}
	~Texture() {
		SOIL_free_image_data(this->data);
		glDeleteTextures(1, &this->texture);
	}
private:
	unsigned char* data;
	int width;
	int height;
};


#endif // TEXTURE_H