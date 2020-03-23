#include "../glm/gtx/transform.hpp"
#include "modelTestScene.h"
#include "../logger.h"

#include "../graphics/primitives.h"
#include "../compat/opengl.h"

ModelTestScene::ModelTestScene() {
	logMessage(INFO, "Seting up the model test scene");

	this->texture = new GlTexture("assets/grid.png");

	logMessage(INFO, "Setup models");
	this->setupModel();

	logMessage(INFO, "Setting up VAO");
	this->setupVao();

	logMessage(INFO, "Setting up shader");
	this->setupShader();

	logMessage(INFO, "Done");
}

ModelTestScene::~ModelTestScene() {
	logMessage(INFO, "Deconstructing...");

	delete this->model;	
}

void ModelTestScene::setupShader() {

	GlShader vertexShader(R"(
attribute vec3 vertexPosition;
attribute vec2 vertexUV;
attribute vec3 normal;

uniform mat4 V;
uniform mat4 P;

varying vec2 UV;

void main(){
    gl_Position = (P*V)*vec4(vertexPosition, 1);
    UV = vertexUV;
}
                          )", GL_VERTEX_SHADER);
    GlShader fragmentShader(R"(
precision mediump float;
varying vec2 UV;
//out vec4 color;

uniform sampler2D diffuseTexture;
void main(){
    //color = texture(diffuseTexture, UV).rgba;
	//color = vec4(1.0, 0.0, 0.0, 1.0);
	gl_FragColor = texture2D(diffuseTexture, UV).rgba;
}
                            )", GL_FRAGMENT_SHADER);

    this->shaderProgram.attachShader(vertexShader);
    this->shaderProgram.attachShader(fragmentShader);
    this->shaderProgram.link();

    this->textureUniformHandle = glGetUniformLocation(this->shaderProgram.getHandle(), "diffuseTexture");

    this->viewMatrixHandle = glGetUniformLocation(this->shaderProgram.getHandle(), "V");
    this->projectionMatrixHandle = glGetUniformLocation(this->shaderProgram.getHandle(), "P");
}

void ModelTestScene::setupModel() {
	this->model = new ObjFile("assets/monke/monkey.obj");

	Mesh* sphere = this->model->getMesh("Suzanne_Suzanne.001");

	//Upload VAO's
	this->vertices = new GlMutableBufferObject(GL_ARRAY_BUFFER);

	this->vertices->upload((const char*)sphere->getVerticePtr(), sizeof(glm::vec3)*sphere->getVerticeCount());

	this->texcoords = new GlMutableBufferObject(GL_ARRAY_BUFFER);

	this->texcoords->upload((const char*)sphere->getTexCoordPtr(), sizeof(glm::vec2)*sphere->getVerticeCount());

	this->normals = new GlMutableBufferObject(GL_ARRAY_BUFFER);

	this->normals->upload((const char*)sphere->getNormalPtr(), sizeof(glm::vec3)*sphere->getVerticeCount());
}

void ModelTestScene::setupVao() {
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

    //Bind normals buffer
    this->normals->bind();
	glVertexAttribPointer(2, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(2);

    //Clean up
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
float timeAccum = 0.0f;

void ModelTestScene::render(uint32_t delta) {
	timeAccum += (float)delta/1000.0f;
	Mesh* sphere = this->model->getMesh("Suzanne_Suzanne.001");

	glDisable(GL_CULL_FACE);
    //glCullFace(GL_BACK);  
    glEnable(GL_DEPTH_TEST);

	glUseProgram(this->shaderProgram.getHandle());

	//Set up camera stuff
	glm::vec3 cameraPos(sin(timeAccum)*10.0f, 0.3f, cos(timeAccum)*4.0f);

    //Matrix stuff
    glm::mat4x4 p = glm::perspective(glm::radians(80.0f), 1280.0f/720.0f, 0.01f, 100.0f);

    glm::mat4x4 v = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glUniformMatrix4fv(this->viewMatrixHandle, 1, GL_FALSE, (const GLfloat*)&v);
    glUniformMatrix4fv(this->projectionMatrixHandle, 1, GL_FALSE, (const GLfloat*)&p);

    //Texture time
    glUniform1i(this->textureUniformHandle, 0);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, this->texture->getHandle());

	glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, sphere->getVerticeCount());

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}
