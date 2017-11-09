// COMP710 GP 2D Framework 2017

// This include:
#include "entity.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"
#include "game.h"

// Library includes:
#include <cassert>

namespace Engine
{
	Entity::Entity()
		: m_pSprite(0)
		, m_x(0.0f)
		, m_y(0.0f)
		, m_velocityX(0.0f)
		, m_velocityY(0.0f)
		, m_dead(false)
	{

	}

	Entity::~Entity()
	{

	}

	bool
		Entity::Initialise(Sprite* sprite)
	{
		assert(sprite);
		m_pSprite = sprite;

		return (true);
	}

	void
		Entity::Process(Game* game, float deltaTime, bool checkCollision)
	{
		if (this->m_velocityX != 0) {
			const int WIDTH = game->GetBackbuffer()->GetWidth();
			float changeFactor = this->m_velocityX * deltaTime;
			if (checkCollision && this->m_velocityX < 0 && this->m_x - changeFactor <= 0) {
				this->m_x = 0;
				this->m_velocityX = 0;
			}
			else if (checkCollision && this->m_velocityX > 0 && this->m_x + this->m_pSprite->GetWidth() + changeFactor >= WIDTH) {
				this->m_x = float(WIDTH - this->m_pSprite->GetWidth());
				this->m_velocityX = 0;
			}
			else {
				this->m_x += this->m_velocityX * deltaTime;
			}
		}

		if (this->m_velocityY != 0) {
			const int HEIGHT = game->GetBackbuffer()->GetHeight();
			float changeFactor = this->m_velocityY * deltaTime;
			if (checkCollision && this->m_velocityY < 0 && this->m_y - changeFactor <= 0) {
				this->m_y = float(m_pSprite->GetHeight() / 2);
				this->m_velocityY = 0;
			}
			else if (checkCollision && this->m_velocityY > 0 && this->m_y + changeFactor >= HEIGHT) {
				this->m_y = float(HEIGHT - m_pSprite->GetHeight() / 2);
				this->m_velocityY = 0;
			}
			else {
				this->m_y += this->m_velocityY * deltaTime;
			}
		}

		if (m_pSprite != nullptr) {
			m_pSprite->Process(deltaTime);
		}
	}

	void
		Entity::Draw(BackBuffer& backBuffer)
	{
		assert(m_pSprite);
		m_pSprite->SetX(static_cast<int>(m_x));
		m_pSprite->SetY(static_cast<int>(m_y));
		m_pSprite->Draw(backBuffer);
	}

	void
		Entity::SetDead(bool dead)
	{
		m_dead = dead;
	}

	bool 
		Entity::IsDead() const {
		return m_dead;
	}

	void
		Entity::SetPosition(float x, float y)
	{
		m_x = x;
		m_y = y;
	}

	void
		Entity::SetPositionX(float x)
	{
		m_x = x;
	}

	void
		Entity::SetPositionY(float y)
	{
		m_y = y;
	}

	float
		Entity::GetPositionX() const
	{
		return (m_x);
	}

	float
		Entity::GetPositionY() const
	{
		return (m_y);
	}

	float
		Entity::GetHorizontalVelocity() const
	{
		return (m_velocityX);
	}

	float
		Entity::GetVerticalVelocity() const
	{
		return (m_velocityY);
	}

	void
		Entity::SetHorizontalVelocity(float x)
	{
		m_velocityX = x;
	}

	void
		Entity::SetVerticalVelocity(float y)
	{
		m_velocityY = y;
	}

	Sprite* 
		Entity::GetSprite() const {
		return m_pSprite;
	}
}