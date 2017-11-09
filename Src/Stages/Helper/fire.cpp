#include "fire.h"

#include "../../Engine/particle.h"
#include "../../Engine/game.h"
#include "../../Engine/sprite.h"
#include "../../Engine/backbuffer.h"

#include <cassert>

Fire::Fire() : ParticleEmitter(150) {}

Engine::Particle* Fire::SpawnNewParticle(Engine::Game* game) {
	Engine::Sprite* sprite = game->GetBackbuffer()->CreateSprite("Assets\\Graphic\\fire.png");
	assert(sprite);

	Engine::Particle* particle = new Engine::Particle(sprite);

	particle->SetPosition(this->GetPositionX(), this->GetPositionY());
	particle->SetHorizontalVelocity(-(float(rand() % 800) - 400));
	particle->SetVerticalVelocity(-(float(600. + rand() % 400)));
	particle->SetLifespan(float(rand() / (RAND_MAX + 0.01)));
	return particle;
}