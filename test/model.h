#ifndef MODEL_H
#pragma once

#include <string>
//#include <assimp/Importer.hpp>
#include <assimp/cimport.h>  
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

class Model {
public:
	Model(const char* path)	{
		this->loadModel(path);
	}
	~Model() {
		aiReleaseImport(this->myScene);
	}
	void draw() {
		if (this->myScene)
			this->processNode(this->myScene->mRootNode, this->myScene);
	}

private:
	const aiScene* myScene;
	void loadModel(const char* path)	{
		
		const aiScene* scene = aiImportFile(path, aiProcess_Triangulate); // aiProcess_FlipUVs
		
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			cout << "ERROR::ASSIMP" << endl;
			return;
		}

		this->myScene = scene;
	}

	void processNode(aiNode* node, const aiScene* scene) {
		aiMatrix4x4 m = node->mTransformation;
		glPushMatrix();
		glMultMatrixf((float*)&m);
		for (GLuint i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];			
			this->processMesh(mesh);
		}

		for (GLuint i = 0; i < node->mNumChildren; i++)	{
			this->processNode(node->mChildren[i], scene);
		}
		glPopMatrix();
	}

	void processMesh(aiMesh* mesh) {

		for (GLuint t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch (face->mNumIndices) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for (GLuint i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				if (mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				if (mesh->mNormals != NULL)
					glNormal3fv(&mesh->mNormals[index].x);
				glVertex3fv(&mesh->mVertices[index].x);
			}

			glEnd();
		}
	}

};


#endif // !MODEL_H