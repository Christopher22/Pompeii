#pragma once

#include "entity.h"

#include <vector>

namespace Engine
{
	class Particle;
	class Game;

	class ParticleEmitter : public Entity {
	public:
		ParticleEmitter(unsigned int particleNumber);
		void Process(Game* game, float deltaTime);
		void Draw(BackBuffer& backBuffer);
		virtual ~ParticleEmitter();

	protected:
		virtual Particle* SpawnNewParticle(Game* game) = 0;

	private:
		std::vector<Particle*> m_particles;
	};
}