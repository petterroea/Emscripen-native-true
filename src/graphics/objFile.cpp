#include <fstream>
#include <sstream>
#include "../glm/vec3.hpp"
#include "../glm/vec2.hpp"
#include "objFile.h"

#include "../logger.h"

ObjFile::ObjFile(std::string filename) {
	this->loadFile(filename);
}

ObjFile::~ObjFile() {
	
}

void ObjFile::loadFile(std::string filename) {
	std::ifstream stream;
	stream.open(filename);
	std::string line;

	//The parse state
	ObjParseState* state = new ObjParseState(); 

	while(std::getline(stream, line))
	{
		std::stringstream ss(line);

		std::string command;
		if(std::getline(ss, command, ' ')) {
			//logMessage(INFO, "Line: " + command);
			if(command.c_str()[0] == '#') {
				logMessage(INFO, "Comment");
			} else if(command == "v") {
				std::string x;
				std::string y;
				std::string z;

				std::getline(ss, x, ' ');
				std::getline(ss, y, ' ');
				std::getline(ss, z, ' ');

				float xf = std::stof(x);
				float yf = std::stof(y);
				float zf = std::stof(z);

				state->pushVertice(glm::vec3(xf, yf, zf));
			} else if(command == "o") {
				std::string objectName;

				std::getline(ss, objectName, ' ');

				state->createObject(objectName);

				logMessage(INFO, "Created new object: " + objectName);
			} else if(command == "mtllib") {
				logMessage(INFO, "Material library");
			} else if(command == "vn") {
				std::string x;
				std::string y;
				std::string z;

				std::getline(ss, x, ' ');
				std::getline(ss, y, ' ');
				std::getline(ss, z, ' ');

				float xf = std::stof(x);
				float yf = std::stof(y);
				float zf = std::stof(z);

				state->pushNormal(glm::vec3(xf, yf, zf));
			} else if(command == "vt") {
				std::string x;
				std::string y;

				std::getline(ss, x, ' ');
				std::getline(ss, y, ' ');

				float xf = std::stof(x);
				float yf = std::stof(y);

				state->pushTexture(glm::vec2(xf, yf));
			} else if(command == "f") {
				std::string a1;
				std::string a2;
				std::string a3;

				std::getline(ss, a1, ' ');
				std::getline(ss, a2, ' ');
				std::getline(ss, a3, ' ');

				state->addTriangle(a1, a2, a3);


				//logMessage(INFO, "Face");
			} else if(command == "usemtl") {
				logMessage(INFO, "Apply material");
			} else if(command == "s") {
				logMessage(INFO, "Smooth shading is on");
			} else {
				logMessage(ERROR, "Unknown command: " + command);
				throw;
			}
		} else {
			logMessage(INFO, "Empty line");
		}
	}
	this->bake(state);
	delete state;
}

void ObjFile::bake(ObjParseState* state) {
	ExpandingArrayBuffer<glm::vec3>* vertices = state->getVertices();
	
	std::map<std::string, UnbakedObject*>::iterator it;
	for(it = state->begin(); it != state->end(); it++) {
		std::string name = it->first;
		UnbakedObject* obj = it->second;

		//Create vertex array
		glm::vec3* destinationVertices = new glm::vec3[obj->vertices->getCount()];
		const int* vIndexes = obj->vertices->getBuffer();
		
		ExpandingArrayBuffer<glm::vec3>* sourceBuffer = state->getVertices();
		const glm::vec3* sourceVertices = sourceBuffer->getBuffer();

		for(int i = 0; i < obj->vertices->getCount(); i++) {
			int index = vIndexes[i]-1;
			destinationVertices[i] = sourceVertices[index];
		}

		Mesh* mesh = new Mesh(destinationVertices, obj->vertices->getCount());

		//Attach normal coords
		if(obj->normals != nullptr) {
			glm::vec3* normals = new glm::vec3[obj->normals->getCount()];
			const int* nIndexes = obj->normals->getBuffer();
			for(unsigned int i = 0; i < obj->normals->getCount(); i++) {
				normals[i] = state->getNormals()->getBuffer()[nIndexes[i]-1];
			}

			mesh->attachNormals(normals, obj->normals->getCount());
		}

		//Attach texture coordinates
		if(obj->texcoords != nullptr) {
			glm::vec2* texcoords = new glm::vec2[obj->texcoords->getCount()];
			const int* tIndexes = obj->texcoords->getBuffer();
			for(unsigned int i = 0; i < obj->texcoords->getCount(); i++) {
				texcoords[i] = state->getTexCoords()->getBuffer()[tIndexes[i]-1];
			}

			mesh->attachTexCoords(texcoords, obj->texcoords->getCount());
		}

		this->meshes[name] = mesh;
	}
}