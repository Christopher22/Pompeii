#pragma once

#include "../../Engine/sprite.h"
#include "../../Engine/backbuffer.h"

#include <string>

class Font;

class Label : public Engine::Sprite {
public:
	struct Color {
		unsigned char Red;
		unsigned char Green;
		unsigned char Blue;

		Color(unsigned char r, unsigned char g, unsigned char b) : Red(r), Green(g), Blue(b) {}
	};

	Label(Font* font, const char* text, float scale = 1.0f);
	~Label();

	void Draw(Engine::BackBuffer& backbuffer);
	int GetWidth() const;
	int GetHeight() const;
	std::string& GetText();
	float GetScale() const;
	void SetScale(float scale);
	void SetColor(Color color);
	void ResetColor();
	Color* GetColor() const;
private:
	const Font* m_pFont;
	Color* m_pColor;
	float m_scale;
	std::string m_text;
};