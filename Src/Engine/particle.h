#pragma once

#include "entity.h"

namespace Engine
{
	class Game;
	class Sprite;

	class Particle : public Entity {
	public:
		Particle(Sprite* sprite, bool checkCollision = false);
		~Particle();

		void Process(Game* game, float deltaTime);

		float GetAge();

		float GetLifespan();
		void SetLifespan(float age);

		float GetAccelerationX();
		float GetAccelerationY();

		void SetAccelerationX(float x);
		void SetAccelerationY(float y);

	private:
		float m_maxAge, m_currentAge;
		float m_accelerationX, m_accelerationY;
		bool m_checkCollision;
	};
}
