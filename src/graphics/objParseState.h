#pragma once

#include <string>
#include <map>

#include "../utils/expandingArrayBuffer.h"

//Used to cache object files before they are loaded
struct UnbakedObject {
	ExpandingArrayBuffer<int>* vertices;
	ExpandingArrayBuffer<int>* normals;
	ExpandingArrayBuffer<int>* texcoords;
};

//Use-and-throw-away state container for parsing
class ObjParseState {
private:
	std::map<std::string, UnbakedObject*> objects;
	UnbakedObject* currentObject;


	ExpandingArrayBuffer<glm::vec3>* vertices = nullptr;
	ExpandingArrayBuffer<glm::vec3>* normals = nullptr;
	ExpandingArrayBuffer<glm::vec2>* textures = nullptr;

	void parseVertice(std::string objVertex, int* v, int* t, int* n);
public:
	ObjParseState();
	~ObjParseState();

	inline void pushVertice(glm::vec3 v) { vertices->push(v); };
	inline void pushNormal(glm::vec3 n) { normals->push(n); };
	inline void pushTexture(glm::vec2 t) { textures->push(t); };

	bool hasObject(const std::string name) const;
	void createObject(const std::string name);

	void addTriangle(std::string p1, std::string p2, std::string p3);

	inline std::map<std::string, UnbakedObject*>::iterator begin() { return objects.begin(); };
	inline std::map<std::string, UnbakedObject*>::iterator end() { return objects.end(); };

	inline ExpandingArrayBuffer<glm::vec3>* getVertices() { return this->vertices; };
	inline ExpandingArrayBuffer<glm::vec3>* getNormals() { return this->normals; };
	inline ExpandingArrayBuffer<glm::vec2>* getTexCoords() { return this->textures; };
};