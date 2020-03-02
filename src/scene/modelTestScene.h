#pragma once
#include "glscene.h"
#include "../graphics/objFile.h"


class ModelTestScene : public GLScene {
private:
	ObjFile* model;
	
	void setupModel();

public:
	ModelTestScene();
	~ModelTestScene();

	void render(uint32_t delta);
};