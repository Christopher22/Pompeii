	// COMP710 GP 2D Framework 2017
#ifndef __ENTITY_H__
#define __ENTITY_H__

namespace Engine
{
	// Forward Declarations:
	class BackBuffer;
	class Sprite;
	class Game;

	class Entity
	{
		//Member Methods:
	public:
		Entity();
		virtual ~Entity();

		bool Initialise(Sprite* sprite);

		virtual void Process(Game* game, float deltaTime, bool checkCollision = true);
		virtual void Draw(BackBuffer& backBuffer);

		void SetDead(bool dead);
		bool IsDead() const;

		void SetPosition(float x, float y);
		void SetPositionX(float x);
		void SetPositionY(float y);

		float GetPositionX() const;
		float GetPositionY() const;

		float GetHorizontalVelocity() const;
		void SetHorizontalVelocity(float x);

		float GetVerticalVelocity() const;
		void SetVerticalVelocity(float y);

		Sprite* GetSprite() const;

	private:
		Entity(const Entity& entity);
		Entity& operator=(const Entity& entity);

	protected:
		Sprite* m_pSprite;

		float m_x;
		float m_y;

		float m_velocityX;
		float m_velocityY;

		bool m_dead;
	};

}
#endif //__ENTITY_H__