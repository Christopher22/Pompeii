#pragma once

#include "../../Engine/texture.h"
#include "../../Engine/backbuffer.h"

class Font {
public:
	Font(Engine::Texture& texture, unsigned short charWidth, unsigned short charHeight, unsigned short charSeparatorX, unsigned short charSeparatorY, unsigned short charDistance, unsigned short charsPerRow);
	void Draw(Engine::BackBuffer& backbuffer, const char* text, int x, int y, float scale) const;
	unsigned short GetCharWidth() const;
	unsigned short GetCharHeight() const;
	unsigned short GetCharSeparatorX() const;
	unsigned short GetCharSeparatorY() const;
	unsigned short GetSpace() const;
	Engine::Texture* GetTexture() const;
private:
	Font(Font& font);
	Engine::Texture* m_pTexture;
	const unsigned short m_charWidth, m_charHeight, m_charsPerRow, m_charSeparatorX, m_charSeparatorY, m_charDistance;
};