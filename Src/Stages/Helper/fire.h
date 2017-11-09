#pragma once

#include "../../Engine/particleEmitter.h"

class Fire : public Engine::ParticleEmitter {
public:
	Fire();
	Engine::Particle* SpawnNewParticle(Engine::Game* game);
};