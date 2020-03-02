#include "softwareParticleEngine.h"
#include "../logger.h"

SoftwareParticleEngine::SoftwareParticleEngine(int maxParticles, glm::vec3 gravity) : maxParticles(maxParticles), gravity(gravity) {
	this->particleVertexBuffer = new glm::vec3[maxParticles];
	this->particleVelocityBuffer = new glm::vec3[maxParticles];
	this->particleSizeBuffer = new float[maxParticles];
	this->particleDeathTime = new float[maxParticles];
	for(int i = 0; i < maxParticles; i++) {
		this->particleDeathTime[i] = -1.0f;
	}
}

SoftwareParticleEngine::~SoftwareParticleEngine() {
	delete[] this->particleVertexBuffer;
	delete[] this->particleVelocityBuffer;
	delete[] this->particleDeathTime;
	delete[] this->particleSizeBuffer;
}

void SoftwareParticleEngine::simulate(uint32_t delta) {
	//logMessage(INFO, "Current particle PTR: " + std::to_string(this->newParticleSeekPointer));
	float deltaf = (float)delta / 1000.0f;
	this->time += deltaf;
	//logMessage(INFO, "Simulation time: " + std::to_string(this->time) + ", particle count " + std::to_string(this->particleCount));
	for(int i = 0; i < this->maxParticles; i++) {
		//Skip non-enabled particles
		if(this->particleDeathTime[i] < 0.0f) continue;
		if(this->particleDeathTime[i] < this->time) {
			this->particleDeathTime[i] = -1.0f;
			this->particleCount--;
			continue;
		}

		//Update velocity
		this->particleVelocityBuffer[i] += this->gravity * deltaf;

		//Update position
		this->particleVertexBuffer[i] += this->particleVelocityBuffer[i];
	}
}

//TODO: This is slow
void SoftwareParticleEngine::spawnParticle(glm::vec3 position, glm::vec3 velocity, float size, float timeOfDeath) {
	if(this->newParticleSeekPointer >= this->maxParticles) {
		this->newParticleSeekPointer = 0;
	}
	//Avoid overflows
	this->particleCount++;
	if(this->particleCount == this->maxParticles) {
		logMessage(INFO, "Too many particles");
		throw "Too many particles! Ask the developer to up the particle limit";
		return;
	}
	//Seek to next free point.
	while(this->particleDeathTime[this->newParticleSeekPointer] >= 0.0f || this->particleDeathTime[this->newParticleSeekPointer] > this->time) { //This might cause slowdowns later
		this->newParticleSeekPointer++;
	}
	//Write particle data
	this->particleVertexBuffer[this->newParticleSeekPointer] = position;
	this->particleVelocityBuffer[this->newParticleSeekPointer] = velocity;
	this->particleSizeBuffer[this->newParticleSeekPointer] = size;

	//Mark memory space as enabled and set up for next particle
	//logMessage(INFO, "New particle with death time " + std::to_string(timeOfDeath));
	this->particleDeathTime[this->newParticleSeekPointer] = timeOfDeath;
	this->newParticleSeekPointer++;

	this->dirtyParticles = true;
}