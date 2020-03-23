#pragma once
#include "glscene.h"
#include "../graphics/objFile.h"

#include "../gl/gltexture.h"
#include "../gl/glmutablebufferobject.h"
#include "../gl/glshaderprogram.h"


class ModelTestScene : public GLScene {
private:
	ObjFile* model;

	GlTexture* texture;

	GLuint vao;

	GlMutableBufferObject* vertices;
	GlMutableBufferObject* normals;
	GlMutableBufferObject* texcoords;

	GlShaderProgram shaderProgram;

	GLuint textureUniformHandle;

	GLuint viewMatrixHandle;
	GLuint projectionMatrixHandle;
	
	void setupModel();
	void setupVao();
	void setupShader();
public:
	ModelTestScene();
	~ModelTestScene();

	void render(uint32_t delta);
};