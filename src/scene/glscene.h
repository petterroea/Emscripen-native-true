#pragma once
#include <cstdint>

class GLScene {
public:
	GLScene();
	~GLScene();

	virtual void render(uint32_t delta) {};
};