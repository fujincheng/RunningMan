#pragma once

#define GLFW_INCLUDE_GLU

#include <vector>
#include <soil.h>
#include <GLFW/glfw3.h>

using namespace std;

class Skybox {
public:
	Skybox() {
		vector<const char*> faces;
		faces.push_back("imgs/skybox/back.jpg");
		faces.push_back("imgs/skybox/front.jpg");
		faces.push_back("imgs/skybox/bottom.jpg");
		faces.push_back("imgs/skybox/top.jpg");
		faces.push_back("imgs/skybox/left.jpg");
		faces.push_back("imgs/skybox/right.jpg");
		loadCubemap(faces);
	}
	~Skybox() {
		for (GLuint i = 0; i < 6; i++) {
			glDeleteTextures(1, &textureID[i]);
		}
	}
	void draw(float x, float y, float z, float width, float height, float length) {
		glDisable(GL_LIGHTING); 

		x = x - width / 2;
		y = y - height / 2;
		z = z - length / 2;

		glPushMatrix();

		/** 绘制背面 */
		glBindTexture(GL_TEXTURE_2D, textureID[1]);

		glBegin(GL_QUADS);

		/** 指定纹理坐标和顶点坐标 */
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y + height, z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y, z);

		glEnd();

		/** 绘制前面 */
		glBindTexture(GL_TEXTURE_2D, textureID[0]);

		glBegin(GL_QUADS);

		/** 指定纹理坐标和顶点坐标 */
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z + length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);

		glEnd();

		/** 绘制底面 */
		glBindTexture(GL_TEXTURE_2D, textureID[2]);

		glBegin(GL_QUADS);

		/** 指定纹理坐标和顶点坐标 */
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);

		glEnd();

		/** 绘制顶面 */
		glBindTexture(GL_TEXTURE_2D, textureID[3]);

		glBegin(GL_QUADS);

		/** 指定纹理坐标和顶点坐标 */
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z);

		glEnd();

		/** 绘制左面 */
		glBindTexture(GL_TEXTURE_2D, textureID[5]);

		glBegin(GL_QUADS);

		/** 指定纹理坐标和顶点坐标 */
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y + height, z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y, z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z);

		glEnd();

		/** 绘制右面 */
		glBindTexture(GL_TEXTURE_2D, textureID[4]);

		glBegin(GL_QUADS);

		/** 指定纹理坐标和顶点坐标 */
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y, z + length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
		glEnd();

		glPopMatrix();

	}
private:
	GLuint textureID[6];
	void loadCubemap(vector<const char*> faces) {
		int width, height;
		unsigned char* image;
		for (GLuint i = 0; i < 6; i++) {
			image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
			glGenTextures(1, &textureID[i]);
			glBindTexture(GL_TEXTURE_2D, textureID[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
		}
	}
};



