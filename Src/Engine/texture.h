// COMP710 GP 2D Framework 2017
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

// Library includes:
#include <SDL.h>

namespace Engine
{
	class Texture
	{
		//Member Methods:
	public:
		Texture();
		~Texture();

		bool Initialise(const char* pcFilename, SDL_Renderer* pRenderer);

		int GetWidth() const;
		int GetHeight() const;

		void SetColor(Uint8 r, Uint8 g, Uint8 b);

		SDL_Texture* GetTexture();

	protected:

	private:
		Texture(const Texture& texture);
		Texture& operator=(const Texture& texture);

		//Member Data:
	public:

	protected:
		SDL_Renderer* m_pRenderer;
		SDL_Texture* m_pTexture;

		int m_width;
		int m_height;

		SDL_RendererFlip m_flip;

	private:

	};
}

#endif // __TEXTURE_H__
