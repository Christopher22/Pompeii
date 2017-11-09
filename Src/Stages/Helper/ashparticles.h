#pragma once

#include "../../Engine/particleEmitter.h"

class AshParticles : public Engine::ParticleEmitter {
public:
	AshParticles();
	Engine::Particle* SpawnNewParticle(Engine::Game* game);
};