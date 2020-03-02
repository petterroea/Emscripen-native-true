#pragma once
#include <cstdint>
#include "../glm/vec3.hpp"

class SoftwareParticleEngine {
private:
	int maxParticles;
	glm::vec3 gravity;

	int newParticleSeekPointer = 0;
	int particleCount = 0;
	float time = 0.0f;
	bool dirtyParticles = false;

	glm::vec3* particleVertexBuffer;
	glm::vec3* particleVelocityBuffer;
	float* particleSizeBuffer;
	float* particleDeathTime;

public:
	SoftwareParticleEngine(int maxParticles, glm::vec3 gravity);
	~SoftwareParticleEngine();

	void simulate(uint32_t delta);
	void spawnParticle(glm::vec3 position, glm::vec3 velocity, float size, float timeOfDeath);

	inline float* getParticlePositionBuffer() { return (float*)this->particleVertexBuffer;}
	inline float* getParticleSizeBuffer() { this->dirtyParticles = false; return (float*)this->particleSizeBuffer; }
	inline float* getParticleDeathTime() { this->dirtyParticles = false; return this->particleDeathTime;}
	inline int getParticleCount() { return this->particleCount; }
	inline int getMaxParticles() { return this->maxParticles; }
	inline float getRenderTime() { return this->time; }

	inline bool hasNewParticles() { return this->dirtyParticles; }
};