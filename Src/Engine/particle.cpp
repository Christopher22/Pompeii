#include "particle.h"

#include "game.h"
#include "backbuffer.h"
#include "sprite.h"

#include <cassert>

Engine::Particle::Particle(Sprite* sprite, bool checkCollision)
: Entity()
, m_currentAge(0)
, m_maxAge(5)
, m_accelerationX(1)
, m_accelerationY(1)
, m_checkCollision(checkCollision)
{
	assert(sprite);
	this->Initialise(sprite);
}

Engine::Particle::~Particle() {
	delete m_pSprite;
	m_pSprite = nullptr;
}

void
Engine::Particle::Process(Game* game, float deltaTime) {
	m_velocityX += m_accelerationX * deltaTime;
	m_velocityY += m_accelerationY * deltaTime;
	m_currentAge += deltaTime;

	if (m_currentAge >= m_maxAge || (m_checkCollision && GetPositionY() >= game->GetBackbuffer()->GetHeight() - GetSprite()->GetHeight())) {
		this->SetDead(true);
	} else {
		Entity::Process(game, deltaTime, m_checkCollision);
	}
}

float 
Engine::Particle::GetAge() {
	return m_currentAge;
}

float 
Engine::Particle::GetLifespan() {
	return m_maxAge;
}

void 
Engine::Particle::SetLifespan(float age) {
	m_maxAge = age;
}

float
Engine::Particle::GetAccelerationX() {
	return m_accelerationX;
}

float
Engine::Particle::GetAccelerationY() {
	return m_accelerationY;
}

void
Engine::Particle::SetAccelerationX(float x) {
	m_accelerationX = x;
}

void
Engine::Particle::SetAccelerationY(float y) {
	m_accelerationY = y;
}