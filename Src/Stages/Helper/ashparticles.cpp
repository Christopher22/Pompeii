#include "ashparticles.h"

#include "../../Engine/particle.h"
#include "../../Engine/game.h"
#include "../../Engine/sprite.h"
#include "../../Engine/backbuffer.h"

#include <cassert>

AshParticles::AshParticles(): ParticleEmitter(120) {}

Engine::Particle* AshParticles::SpawnNewParticle(Engine::Game* game) {
	Engine::Sprite* sprite = game->GetBackbuffer()->CreateSprite("Assets\\Graphic\\ash.png");
	assert(sprite);

	Engine::Particle* particle = new Engine::Particle(sprite, true);

	particle->SetPosition(float(rand() % game->GetBackbuffer()->GetWidth()), 0);
	particle->SetVerticalVelocity(float(rand() % 60));

	float horizontalSpeed = float(rand() % 20);
	particle->SetHorizontalVelocity(horizontalSpeed > 10 ? horizontalSpeed - 20 : horizontalSpeed);
	particle->SetLifespan(20);
	return particle;
}