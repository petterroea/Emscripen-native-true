#include "mesh.h"

#include "../logger.h"

Mesh::Mesh(glm::vec3* vertices, int verticeCount) : vertices(vertices), 
													verticeCount(verticeCount){

}
Mesh::~Mesh() {
	delete vertices;
	if(normals != nullptr) {
		delete normals;
	}
	if(texCoords != nullptr) {
		delete texCoords;
	}
}

void Mesh::attachNormals(glm::vec3* normals, int normalCount) {
	if(this->normals != nullptr) {
		logMessage(ERROR, "Failed to attach normals to mesh - already attached!");
		throw;
	}

	this->normals = normals;
	this->normalCount = normalCount;
}

void Mesh::attachTexCoords(glm::vec2* texCoords, int texCoordCount) {
	if(this->texCoords != nullptr) {
		logMessage(ERROR, "Failed to attach texture coordinates - already attached");
		throw;
	}

	this->texCoords = texCoords;
	this->texCoordCount = texCoordCount;
}