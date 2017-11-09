#include "label.h"

#include "font.h"
#include "../../Engine/texture.h"

#include <cassert>

Label::Label(Font* font, const char* text, float scale) : Sprite(), m_pFont(font), m_pColor(nullptr), m_scale(scale), m_text(text) {
	bool iniSuccessfull = this->Initialise(*font->GetTexture());
	assert(iniSuccessfull);
}

Label::~Label() {
	if (m_pColor) {
		delete m_pColor;
		m_pColor = nullptr;
	}
}

void Label::Draw(Engine::BackBuffer& backbuffer) {
	if (m_pColor) {
		this->GetTexture()->SetColor(m_pColor->Red, m_pColor->Green, m_pColor->Blue);
	}

	m_pFont->Draw(backbuffer, m_text.c_str(), this->GetX(), this->GetY(), m_scale);

	if (m_pColor) {
		this->GetTexture()->SetColor(255, 255, 255);
	}
}

void Label::ResetColor() {
	if (m_pColor) {
		delete m_pColor;
	}
	m_pColor = nullptr;
}

void Label::SetColor(Color color) {
	if (m_pColor) {
		delete m_pColor;
	}
	m_pColor = new Color(color);
}

Label::Color* Label::GetColor() const {
	return m_pColor;
}

int Label::GetWidth() const {
	return m_text.length() * int(m_pFont->GetSpace() * m_scale);
}

int Label::GetHeight() const {
	return int(m_pFont->GetCharHeight() * m_scale);
}

std::string& Label::GetText() {
	return m_text;
}

float Label::GetScale() const {
	return m_scale;
}

void Label::SetScale(float scale) {
	m_scale = scale;
}