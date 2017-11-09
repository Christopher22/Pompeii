// COMP710 GP 2D Framework 2017

// This include:
#include "backbuffer.h"

// Local includes:
#include "logmanager.h"
#include "texturemanager.h"
#include "sprite.h"
#include "texture.h"

// Library includes:
#include <SDL.h>
#include <SDL_image.h>
#include <cassert>

namespace Engine {

	BackBuffer::BackBuffer()
		: m_pTextureManager(0)
		, m_pWindow(0)
		, m_pRenderer(0)
		, m_width(0)
		, m_height(0)
		, m_clearRed(0xFF)
		, m_clearGreen(0xFF)
		, m_clearBlue(0xFF)
		, m_camX(0)
		, m_camY(0)
	{

	}

	BackBuffer::~BackBuffer()
	{
		::SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;

		::SDL_DestroyWindow(m_pWindow);
		m_pWindow = nullptr;

		delete m_pTextureManager;
		m_pTextureManager = nullptr;

		IMG_Quit();
		SDL_Quit();
	}

	bool
		BackBuffer::Initialise(const char* title, unsigned short width, unsigned short height)
	{
		m_width = width;
		m_height = height;

		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
		{
			LogSDLError();
			return (false);
		}
		else
		{
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

			m_pWindow = ::SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)width, (int)height, SDL_WINDOW_SHOWN);

			if (m_pWindow == 0)
			{
				LogSDLError();
				return (false);
			}
			else
			{
				m_pRenderer = ::SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);

				if (m_pRenderer == 0)
				{
					LogSDLError();
					return (false);
				}
				else
				{
					int imageResult = IMG_Init(IMG_INIT_PNG);

					if (imageResult == 0)
					{
						LogSDLError();
						return (false);
					}
				}
			}
		}

		m_pTextureManager = new TextureManager();
		assert(m_pTextureManager);
		m_pTextureManager->Initialise(m_pRenderer);

		return (true);
	}

	void
		BackBuffer::Clear()
	{
		SDL_SetRenderDrawColor(m_pRenderer, m_clearRed, m_clearGreen, m_clearBlue, 0xff);
		SDL_RenderClear(m_pRenderer);
	}

	void
		BackBuffer::Present()
	{
		::SDL_RenderPresent(m_pRenderer);
	}

	void
		BackBuffer::SetDrawColour(unsigned char r, unsigned char g, unsigned char b)
	{
		SetDrawColour(r, g, b, 0xFF);
	}

	void
		BackBuffer::SetDrawColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		::SDL_SetRenderDrawColor(m_pRenderer, r, g, b, a);
	}

	void
		BackBuffer::DrawSprite(Sprite& sprite)
	{
		::SDL_Rect dest;

		dest.x = sprite.GetX() - m_camX;
		dest.y = sprite.GetY() - m_camY;
		dest.w = sprite.GetWidth();
		dest.h = sprite.GetHeight();

		SDL_RenderCopy(m_pRenderer, sprite.GetTexture()->GetTexture(), 0, &dest);
	}

	void 
		BackBuffer::DrawSprite(Sprite& sprite, int sourceX, int sourceY, int sourceW, int sourceH, bool absolut)
	{
		::SDL_Rect source, dest;

		source.x = sourceX;
		source.y = sourceY;
		source.w = sourceW;
		source.h = sourceH;

		dest.x = sprite.GetX() - (absolut ? 0 : m_camX);
		dest.y = sprite.GetY() - (absolut ? 0 : m_camY);
		dest.w = sprite.GetWidth();
		dest.h = sprite.GetHeight();

		SDL_RenderCopy(m_pRenderer, sprite.GetTexture()->GetTexture(), &source, &dest);
	}

	void 
		BackBuffer::DrawTexture(Texture* texture, int x, int y) 
	{
		::SDL_Rect dest;

		dest.x = x - m_camX;
		dest.y = y - m_camY;
		dest.w = texture->GetWidth();
		dest.h = texture->GetHeight();

		SDL_RenderCopy(m_pRenderer, texture->GetTexture(), 0, &dest);
	}

	void 
		BackBuffer::DrawTexture(Texture* texture, int sourceX, int sourceY, int sourceW, int sourceH, int targetX, int targetY, int targetW, int targetH, bool absolut) 
	{	
		::SDL_Rect source, dest;

		source.x = sourceX;
		source.y = sourceY;
		source.w = sourceW;
		source.h = sourceH;

		dest.x = targetX - (absolut ? 0 : m_camX);
		dest.y = targetY - (absolut ? 0 : m_camY);
		dest.w = targetW;
		dest.h = targetH;

		SDL_RenderCopy(m_pRenderer, texture->GetTexture(), &source, &dest);
	}

	void
		BackBuffer::DrawRectangle(int x1, int y1, int x2, int y2)
	{
		::SDL_Rect fillRect;

		fillRect.x = x1 - m_camX;
		fillRect.y = y1 - m_camY;
		fillRect.w = x2 - x1;
		fillRect.h = y2 - y1;

		::SDL_RenderFillRect(m_pRenderer, &fillRect);
	}

	void
		BackBuffer::DrawLine(int x1, int y1, int x2, int y2)
	{
		::SDL_RenderDrawLine(m_pRenderer, x1 - m_camX, y1 - m_camY, x2 - m_camX, y2 - m_camY);
	}

	void
		BackBuffer::LogSDLError()
	{
		LogManager::GetInstance().Log(SDL_GetError());
	}

	Sprite*
		BackBuffer::CreateSprite(const char* pcFilename)
	{
		assert(m_pTextureManager);

		Texture* pTexture = m_pTextureManager->GetTexture(pcFilename);

		Sprite* pSprite = new Sprite();
		if (!pSprite->Initialise(*pTexture))
		{
			LogManager::GetInstance().Log("Sprite Failed to Create!");
		}

		return (pSprite);
	}

	Texture* BackBuffer::CreateUnmanagedTexture(const char* pcFilename) {
		Texture* tmp = new Texture();
		if (tmp->Initialise(pcFilename, m_pRenderer)) {
			return tmp;
		}
		else {
			delete tmp;
			tmp = nullptr;
			return nullptr;
		}
	}

	void
		BackBuffer::SetClearColour(unsigned char r, unsigned char g, unsigned char b)
	{
		m_clearRed = r;
		m_clearGreen = g;
		m_clearBlue = b;
	}

	TextureManager* 
		BackBuffer::GetTextureManager() const {
		return m_pTextureManager;
	}

	unsigned short
		BackBuffer::GetWidth() const {
		return m_width;
	}

	unsigned short
		BackBuffer::GetHeight() const {
		return m_height;
	}

	void BackBuffer::setCamera(unsigned short x, unsigned short y) {
		m_camX = x;
		m_camY = y;
	}

	unsigned short BackBuffer::getCameraX() const {
		return m_camX;
	}

	unsigned short BackBuffer::getCameraY() const {
		return m_camY;
	}
}