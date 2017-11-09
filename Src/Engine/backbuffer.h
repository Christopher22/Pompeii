// COMP710 GP 2D Framework 2017
#ifndef __BACKBUFFER_H__
#define __BACKBUFFER_H__

struct SDL_Window;
struct SDL_Renderer;

namespace Engine {
	class TextureManager;
	class Texture;
	class Sprite;
	
	class BackBuffer
	{
		//Member Methods:
	public:
		BackBuffer();
		~BackBuffer();

		bool Initialise(const char* title, unsigned short width, unsigned short height);

		void Clear();
		void Present();

		void SetClearColour(unsigned char r, unsigned char g, unsigned char b);

		void SetDrawColour(unsigned char r, unsigned char g, unsigned char b);
		void SetDrawColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

		void DrawSprite(Sprite& sprite);
		void DrawSprite(Sprite& sprite, int sourceX, int sourceY, int sourceW, int sourceH, bool absolut);
		void DrawTexture(Texture* texture, int x, int y);
		void DrawTexture(Texture* texture, int sourceX, int sourceY, int sourceW, int sourceH, int targetX, int targetY, int targetW, int targetH, bool absolut);

		void DrawRectangle(int x1, int y1, int x2, int y2);
		void DrawLine(int x1, int y1, int x2, int y2);

		Sprite* CreateSprite(const char* pcFilename);
		Texture* CreateUnmanagedTexture(const char* pcFilename);

		unsigned short GetWidth() const;
		unsigned short GetHeight() const;

		void setCamera(unsigned short x, unsigned short y);
		unsigned short getCameraX() const;
		unsigned short getCameraY() const;

		TextureManager* GetTextureManager() const;
	protected:
		void LogSDLError();

	private:
		BackBuffer(const BackBuffer& backBuffer);
		BackBuffer& operator=(const BackBuffer& backBuffer);

		//Member Data:
	public:

	protected:
		TextureManager* m_pTextureManager;

		::SDL_Window* m_pWindow;
		::SDL_Renderer* m_pRenderer;

		unsigned short m_width;
		unsigned short m_height;

		unsigned char m_clearRed;
		unsigned char m_clearGreen;
		unsigned char m_clearBlue;

		unsigned short m_camX;
		unsigned short m_camY;
	private:

	};
}
#endif // __BACKBUFFER_H__
