#include <fstream>
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
	while(std::getline(stream, line))
	{
		logMessage(INFO, "Line: " + line);
	}
}