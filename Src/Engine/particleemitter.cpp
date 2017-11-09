#include "particleEmitter.h"

#include "particle.h"

#include <algorithm>

Engine::ParticleEmitter::ParticleEmitter(unsigned int particleNumber)
: Entity()
, m_particles(particleNumber, nullptr)
{}

Engine::ParticleEmitter::~ParticleEmitter() {
	std::for_each(m_particles.begin(), m_particles.end(), [](Particle* particle) {
		delete particle;
		particle = nullptr;
	});
}

void Engine::ParticleEmitter::Draw(BackBuffer& backBuffer) {
	std::for_each(m_particles.begin(), m_particles.end(), [&](Particle* particle) {
		if (particle != nullptr) {
			particle->Draw(backBuffer);
		}
	});
}

void Engine::ParticleEmitter::Process(Game* game, float deltaTime) {
	auto particle = m_particles.begin();
	while (particle != m_particles.end()) {
		if (*particle == nullptr || ((*particle)->IsDead())) {
			delete *particle; // Deleting nullptr is safe
			*particle = this->SpawnNewParticle(game);
		} else {
			(*particle)->Process(game, deltaTime);
		}
		++particle;
	}
	Entity::Process(game, deltaTime);
}