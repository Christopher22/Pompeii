#include "stage.h"

#include <cassert>

Stage::Stage(Font* font) : m_pFont(font) {
	assert(font);
}

Stage::~Stage() {}

Font* Stage::GetFont() const {
	return m_pFont;
}