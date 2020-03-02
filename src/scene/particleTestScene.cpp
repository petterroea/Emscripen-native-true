#include "../glm/gtx/transform.hpp"
#include "particleTestScene.h"
#include "../logger.h"

#include "../graphics/primitives.h"
#include "../compat/opengl.h"

ParticleTestScene::ParticleTestScene() {
	logMessage(INFO, "Seting up the particle test scene");

	this->texture = new GlTexture("assets/smoke.png");

	this->particleSystem = new SoftwareParticleEngine(10000, glm::vec3(0.0f, 0.0f, 0.0f));

	logMessage(INFO, "Setup models");
	this->setupModel();

	logMessage(INFO, "Init VAO");
	this->setupVao();

	logMessage(INFO, "Init shader");
	this->setupShader();

	logMessage(INFO, "Done");
}

ParticleTestScene::~ParticleTestScene() {
	logMessage(INFO, "Deconstructing...");
	delete this->texture;

	delete this->vertices;
	delete this->texcoords;
	delete this->particlePoints;
	delete this->particleSize;

	glDeleteVertexArrays(1, &this->vao);
}

void ParticleTestScene::setupShader() {

	GlShader vertexShader(R"(
attribute vec3 vertexPosition;
attribute vec2 vertexUV;
attribute vec3 particlePosition;
attribute float deathTime;
attribute float size;

varying vec2 UV;
varying float dt;

uniform mat4 V;
uniform mat4 P;
uniform vec3 gCameraPos;

void main(){
	dt = deathTime;

	vec3 CameraRight_worldspace = vec3(V[0][0], V[1][0], V[2][0]);
	vec3 CameraUp_worldspace = vec3(V[0][1], V[1][1], V[2][1]);

	vec3 vertexPosition_worldspace =
    particlePosition
    + CameraRight_worldspace * vertexPosition.x * size
    + CameraUp_worldspace * vertexPosition.y * size;

    gl_Position = ((P*V)*vec4(vertexPosition_worldspace, 1.0));
    UV = vertexUV;
}
                          )", GL_VERTEX_SHADER);

    GlShader fragmentShader(R"(
precision mediump float;
varying vec2 UV;
varying float dt;

uniform float currentTime;

uniform sampler2D diffuseTexture;
void main(){
	if(dt < 0.0 || (dt < currentTime && dt != 0.0)) discard;
    //color = texture(diffuseTexture, UV).rgba;
	//color = vec4(1.0, 0.0, 0.0, 1.0);
	gl_FragColor = texture2D(diffuseTexture, UV).rgba;
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
                            )", GL_FRAGMENT_SHADER);

    this->shaderProgram.attachShader(vertexShader);
    this->shaderProgram.attachShader(fragmentShader);
    this->shaderProgram.link();

    this->textureUniformHandle = glGetUniformLocation(this->shaderProgram.getHandle(), "diffuseTexture");
    this->cameraPosHandle = glGetUniformLocation(this->shaderProgram.getHandle(), "gCameraPos");
    this->currentTimeHandle = glGetUniformLocation(this->shaderProgram.getHandle(), "currentTime");

    this->viewMatrixHandle = glGetUniformLocation(this->shaderProgram.getHandle(), "V");
    this->projectionMatrixHandle = glGetUniformLocation(this->shaderProgram.getHandle(), "P");
}

void ParticleTestScene::setupModel() {
	this->vertices = new GlMutableBufferObject(GL_ARRAY_BUFFER);

	this->vertices->upload((const char*)primitive_face, sizeof(float)*3*6);

	this->texcoords = new GlMutableBufferObject(GL_ARRAY_BUFFER);

	this->texcoords->upload((const char*)primitive_face_texcoords, sizeof(float)*2*6);

	//Coordinates
	this->particlePoints = new GlMutableBufferObject(GL_ARRAY_BUFFER);

	//Is the particle enabled?
	this->particleDeathTime = new GlMutableBufferObject(GL_ARRAY_BUFFER);

	this->particleSize = new GlMutableBufferObject(GL_ARRAY_BUFFER);
}

void ParticleTestScene::setupVao() {
	//Set up VAO
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	//Bind vertice buffer
	this->vertices->bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(0);

    //Bind texcoord buffer
    this->texcoords->bind();
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(1);

    //Particle position buffer
    this->particlePoints->bind();
	glVertexAttribPointer(2, 3, GL_FLOAT, false, 0, 0);
    glVertexAttribDivisor(2, 1);
    glEnableVertexAttribArray(2);

    //Particle enabled buffer
    this->particleDeathTime->bind();
	glVertexAttribPointer(3, 1, GL_FLOAT, false, 0, 0);
    glVertexAttribDivisor(3, 1);
    glEnableVertexAttribArray(3);

    //Particle enabled buffer
    this->particleSize->bind();
	glVertexAttribPointer(4, 1, GL_FLOAT, false, 0, 0);
    glVertexAttribDivisor(4, 1);
    glEnableVertexAttribArray(4);

    //Clean up
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ParticleTestScene::uploadParticles() {
	this->particlePoints->upload((const char*)this->particleSystem->getParticlePositionBuffer(), this->particleSystem->getMaxParticles()*sizeof(glm::vec3));

	if(this->particleSystem->hasNewParticles()) {
		this->particleDeathTime->upload((const char*)this->particleSystem->getParticleDeathTime(), this->particleSystem->getMaxParticles()*sizeof(float));
		this->particleSize->upload((const char*)this->particleSystem->getParticleSizeBuffer(), this->particleSystem->getMaxParticles()*sizeof(float));
	}
}
float t = 0.0f;
void ParticleTestScene::render(uint32_t delta) {
	t += (float)delta/1000.0f;
	for(int i = 0; i < 10; i++) {
		this->spawnParticle();
	}
	this->particleSystem->simulate(delta);

	this->uploadParticles();

    glUseProgram(this->shaderProgram.getHandle());

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    //Bind texture
    glUniform1i(this->textureUniformHandle, 0);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, this->texture->getHandle());

    glm::vec3 cameraPos(sin(t)*70.0f, 0.3f, cos(t)*40.0f);

    //Matrix stuff
    glm::mat4x4 p = glm::perspective(glm::radians(80.0f), 1280.0f/720.0f, 0.01f, 100.0f);

    glm::mat4x4 v = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glUniformMatrix4fv(this->viewMatrixHandle, 1, GL_FALSE, (const GLfloat*)&v);
    glUniformMatrix4fv(this->projectionMatrixHandle, 1, GL_FALSE, (const GLfloat*)&p);

    glUniform3fv(this->cameraPosHandle, 1, (const GLfloat*)&cameraPos);

    glUniform1f(this->currentTimeHandle, this->particleSystem->getRenderTime());

    //Draw
	glBindVertexArray(this->vao);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, this->particleSystem->getMaxParticles());

    //Cleanup
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void ParticleTestScene::spawnParticle() {
	float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float t = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	float xp = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float yp = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float zp = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	float sz = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	this->particleSystem->spawnParticle(glm::vec3(xp-0.5f, yp-0.5f, zp-0.5f), glm::vec3(x*0.2f-0.1f, y*0.2f-0.1f, z*0.2f-0.1f), sz+0.5f, 10.0f+t*4.0f+this->particleSystem->getRenderTime());
}