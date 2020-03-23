#pragma once
#include <string>
#include <map>

#include "../utils/expandingArrayBuffer.h"
#include "objParseState.h"
#include "mesh.h"

class ObjFile {
private:
	std::map<std::string, Mesh*> meshes;

	void loadFile(std::string filename);
	void bake(ObjParseState* state);
public:
	ObjFile(std::string filename);
	~ObjFile();

	inline std::map<std::string, Mesh*>::iterator begin() { return meshes.begin(); };
	inline std::map<std::string, Mesh*>::iterator end() { return meshes.end(); };

	inline bool hasObject(const std::string name) { return meshes.find(name) != meshes.end(); };
	inline Mesh* getMesh(const std::string name) { return meshes[name]; };
};