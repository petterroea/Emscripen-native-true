#include "../glm/gtx/transform.hpp"
#include "modelTestScene.h"
#include "../logger.h"

#include "../graphics/primitives.h"
#include "../compat/opengl.h"

ModelTestScene::ModelTestScene() {
	logMessage(INFO, "Seting up the model test scene");

	logMessage(INFO, "Setup models");
	this->setupModel();

	logMessage(INFO, "Done");
}

ModelTestScene::~ModelTestScene() {
	logMessage(INFO, "Deconstructing...");

	delete this->model;	

}


void ModelTestScene::setupModel() {
	this->model = new ObjFile("assets/rock/untitled.obj");
}

void ModelTestScene::render(uint32_t delta) {
	
}
