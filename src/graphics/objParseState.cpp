#include "objParseState.h"

#include <sstream>

#include "../logger.h"

ObjParseState::ObjParseState() {
	vertices = new ExpandingArrayBuffer<glm::vec3>(1024);
	normals = new ExpandingArrayBuffer<glm::vec3>(1024);
	textures = new ExpandingArrayBuffer<glm::vec2>(1024);
}

ObjParseState::~ObjParseState() {
	delete vertices;
	delete normals;
	delete textures;
	std::map<std::string, UnbakedObject*>::iterator it;
	for(it = this->objects.begin(); it != this->objects.end(); it++) {
		delete (*it).second;
	}
}

bool ObjParseState::hasObject(const std::string name) const{
	return this->objects.find(name) != this->objects.end();
}

void ObjParseState::createObject(const std::string name){
	if(this->hasObject(name)) {
		logMessage(ERROR, "Failed to parse OBJ: Object " + name + " already exists");
		throw;
	}
	logMessage(INFO, "New object: " + name);
	this->currentObject = new UnbakedObject;
	this->currentObject->vertices = nullptr;
	this->currentObject->normals = nullptr;
	this->currentObject->texcoords = nullptr;

	this->objects[name] = this->currentObject;
}

void ObjParseState::addTriangle(std::string p1, std::string p2, std::string p3) {
	int p1v, p1t, p1n;
	int p2v ,p2t, p2n;
	int p3v, p3t, p3n;

	this->parseVertice(p1, &p1v, &p1t, &p1n);
	this->parseVertice(p2, &p2v, &p2t, &p2n);
	this->parseVertice(p3, &p3v, &p3t, &p3n);

	//Consistency checking
	if((p1t == 0 || p2t == 0 || p3t == 0) && (p1t+p2t+p3t) != 0) {
		logMessage(ERROR, "Failed to parse object file: some vertices contain texture coordinates, others don't");
		throw;
	}

	if((p1n == 0 || p2n == 0 || p3n == 0) && (p1n+p2n+p3n) != 0) {
		logMessage(ERROR, "Failed to parse object file: some vertices contain normal coordinates, others don't");
		throw;
	}

	if(this->currentObject->vertices == nullptr) {
		//Set up the object
		this->currentObject->vertices = new ExpandingArrayBuffer<int>(1024);

		this->currentObject->vertices->push(p1v);
		this->currentObject->vertices->push(p2v);
		this->currentObject->vertices->push(p3v);

		if((p1t+p2t+p3t) != 0) {
			this->currentObject->texcoords = new ExpandingArrayBuffer<int>(1024);

			this->currentObject->texcoords->push(p1t);
			this->currentObject->texcoords->push(p2t);
			this->currentObject->texcoords->push(p3t);
		}
		if((p1n+p2n+p3n) != 0) {
			this->currentObject->normals = new ExpandingArrayBuffer<int>(1024);

			this->currentObject->normals->push(p1n);
			this->currentObject->normals->push(p2n);
			this->currentObject->normals->push(p3n);
		}
	} else {
		if((p1t != 0 || p2t != 0 || p3t != 0) && this->currentObject->texcoords == nullptr) {
			logMessage(ERROR, "Failde to parse object file: Suddently started providing texcoords in the middle of an object");
			throw;
		}

		if((p1n != 0 || p2n != 0 || p3n != 0) && this->currentObject->normals == nullptr) {
			logMessage(ERROR, "Failde to parse object file: Suddently started providing normals in the middle of an object");
			throw;
		}

		//Push indices
		this->currentObject->vertices->push(p1v);
		this->currentObject->vertices->push(p2v);
		this->currentObject->vertices->push(p3v);

		if(p1t != 0) {
			this->currentObject->texcoords->push(p1t);
			this->currentObject->texcoords->push(p2t);
			this->currentObject->texcoords->push(p3t);
		}

		if(p1n != 0) {
			this->currentObject->normals->push(p1n);
			this->currentObject->normals->push(p2n);
			this->currentObject->normals->push(p3n);
		}
	}
}


void ObjParseState::parseVertice(std::string objVertex, int* v, int* t, int* n) {
	std::stringstream args(objVertex);

	//Parse X component
	std::string vs;
	std::getline(args, vs, '/');
	
	std::string ts;
	std::getline(args, ts, '/');
	
	std::string ns;
	std::getline(args, ns, '/');

	*v = std::stoi(vs.c_str());
	*t = std::stoi(ts.c_str());
	*n = std::stoi(ns.c_str());
	
	//logMessage(INFO, "Vertex with indices " + vs + ":" + ts + ":" + ns);
}