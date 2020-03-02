#pragma once
#include "glscene.h"
#include "../gl/gltexture.h"
#include "../gl/glmutablebufferobject.h"
#include "../gl/glshaderprogram.h"

#include "../engines/softwareParticleEngine.h"


class ParticleTestScene : public GLScene {
private:
	GlTexture* texture;

	GLuint vao;

	GlMutableBufferObject* vertices;
	GlMutableBufferObject* texcoords;
	GlMutableBufferObject* particlePoints;
	GlMutableBufferObject* particleSize;
	GlMutableBufferObject* particleDeathTime;

	SoftwareParticleEngine* particleSystem;

	GlShaderProgram shaderProgram;

	GLuint textureUniformHandle;
	
	GLuint viewMatrixHandle;
	GLuint projectionMatrixHandle;

	GLuint cameraPosHandle;
	GLuint currentTimeHandle;

	void setupModel();
	void setupVao();
	void setupShader();

	void uploadParticles();

	void spawnParticle();

public:
	ParticleTestScene();
	~ParticleTestScene();

	void render(uint32_t delta);
};