// COMP710 GP 2D Framework 2017
#ifndef __SPRITE_H__
#define __SPRITE_H__

namespace Engine
{
	// Forward Declarations:
	class BackBuffer;
	class Texture;

	class Sprite
	{
		//Member Methods:
	public:
		Sprite();
		virtual ~Sprite();

		bool Initialise(Texture& texture);

		virtual void Process(float deltaTime);
		virtual void Draw(BackBuffer& backbuffer);

		void SetX(int x);
		void SetY(int y);

		int GetX() const;
		int GetY() const;

		void SetAngle(float angle);
		float GetAngle() const;

		void SetCenter(int x, int y);
		void SetHandleCenter();

		virtual int GetWidth() const;
		virtual int GetHeight() const;

		Texture* GetTexture();

	protected:

	private:
		Sprite(const Sprite& sprite);
		Sprite& operator=(const Sprite& sprite);

		//Member Data:
	public:

	protected:
		Texture* m_pTexture;
		int m_x;
		int m_y;

		float m_angle;
		int m_centerX;
		int m_centerY;

		int m_width;
		int m_height;

	private:

	};
}

#endif // __SPRITE_H__
