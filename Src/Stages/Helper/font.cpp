#include "font.h"

Font::Font(Engine::Texture& texture, unsigned short charWidth, unsigned short charHeight, unsigned short charSeparatorX, unsigned short charSeparatorY, unsigned short charDistance, unsigned short charsPerRow) : m_pTexture(&texture), m_charWidth(charWidth), m_charHeight(charHeight), m_charsPerRow(charsPerRow), m_charSeparatorX(charSeparatorX), m_charSeparatorY(charSeparatorY), m_charDistance(charDistance) {
}

void Font::Draw(Engine::BackBuffer& backbuffer, const char* text, int x, int y, float scale) const {
	x -= int(m_charDistance * scale) / 2;
	for (const char* it = text; *it; ++it) {
		unsigned char currentChar = unsigned char(*it) - 32;
		auto charX = (currentChar % m_charsPerRow) * (m_charWidth + m_charSeparatorX), charY = (currentChar / m_charsPerRow) * (m_charHeight + m_charSeparatorY);
		if (charX + m_charWidth <= m_pTexture->GetWidth() && charY + m_charHeight <= m_pTexture->GetHeight()) {
			backbuffer.DrawTexture(m_pTexture, charX, charY, m_charWidth + 1, m_charHeight + 1, x, y, int(m_charWidth * scale), int(m_charHeight * scale), true);
			x += int(m_charDistance * scale);
		}
	}
}

unsigned short Font::GetCharWidth() const {
	return m_charWidth;
}

unsigned short Font::GetCharHeight() const {
	return m_charHeight;
}

unsigned short Font::GetCharSeparatorX() const {
	return m_charSeparatorX;
}

unsigned short Font::GetCharSeparatorY() const {
	return m_charSeparatorY;
}

unsigned short Font::GetSpace() const {
	return m_charDistance;
}

Engine::Texture* Font::GetTexture() const {
	return m_pTexture;
}