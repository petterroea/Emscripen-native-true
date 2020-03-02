#include "textureTestScene.h"
#include "../logger.h"

#include "../graphics/primitives.h"

TextureTestScene::TextureTestScene() {
	logMessage(INFO, "Seting up the texture test scene");

	this->texture = new GlTexture("assets/grid.png");

	logMessage(INFO, "Setup models");
	this->setupModel();

	logMessage(INFO, "Init VAO");
	this->setupVao();

	logMessage(INFO, "Init shader");
	this->setupShader();

	logMessage(INFO, "Done");
}

TextureTestScene::~TextureTestScene() {
	logMessage(INFO, "Deconstructing...");
	delete this->texture;

	delete this->vertices;
	delete this->texcoords;

	glDeleteVertexArrays(1, &this->vao);
}

void TextureTestScene::setupShader() {

	GlShader vertexShader(R"(
attribute vec3 vertexPosition;
attribute vec2 vertexUV;

varying vec2 UV;

void main(){
    gl_Position = vec4(vertexPosition, 1);
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
}

void TextureTestScene::setupModel() {
	this->vertices = new GlMutableBufferObject(GL_ARRAY_BUFFER);

	this->vertices->upload((const char*)primitive_face, sizeof(primitive_face));

	this->texcoords = new GlMutableBufferObject(GL_ARRAY_BUFFER);

	this->texcoords->upload((const char*)primitive_face_texcoords, sizeof(primitive_face_texcoords));
}

void TextureTestScene::setupVao() {
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

    //Clean up
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextureTestScene::render(uint32_t delta) {
    glUseProgram(this->shaderProgram.getHandle());

    glUniform1i(this->textureUniformHandle, 0);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, this->texture->getHandle());

	glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}