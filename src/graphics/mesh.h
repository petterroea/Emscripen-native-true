#pragma once

#include "../glm/vec3.hpp"
#include "../glm/vec2.hpp"

class Mesh {
private:
	glm::vec3* vertices = nullptr;
	glm::vec3* normals = nullptr;
	glm::vec2* texCoords = nullptr;

	int verticeCount = 0;
	int normalCount = 0;
	int texCoordCount = 0;
public:
	Mesh(glm::vec3* vertices, int verticeCount);
	~Mesh();

	void attachNormals(glm::vec3* normals, int normalCount);
	void attachTexCoords(glm::vec2* texCoords, int texCoordCount);

	inline glm::vec3* getVerticePtr() const { return this->vertices; };
	inline glm::vec3* getNormalPtr() const { return this->normals; };
	inline glm::vec2* getTexCoordPtr() const { return this->texCoords; };

	inline int getVerticeCount() const { return this->verticeCount; };
	inline int getNormalCount() const { return this->normalCount; };
	inline int getTexCoordCount() const { return this->texCoordCount; };
};