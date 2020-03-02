#pragma once
#include <string>

class ObjFile {
private:
	void loadFile(std::string filename);
public:
	ObjFile(std::string filename);
	~ObjFile();
};