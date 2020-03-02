#pragma once
#include "glscene.h"
#include "../gl/gltexture.h"
#include "../gl/glmutablebufferobject.h"
#include "../gl/glshaderprogram.h"

class TextureTestScene : public GLScene {
private:
	GlTexture* texture;

	GLuint vao;

	GlMutableBufferObject* vertices;
	GlMutableBufferObject* texcoords;

	GlShaderProgram shaderProgram;

	GLuint textureUniformHandle;

	void setupModel();
	void setupVao();
	void setupShader();
public:
	TextureTestScene();
	~TextureTestScene();

	void render(uint32_t delta);
};