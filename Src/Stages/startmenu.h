#pragma once

#include "menu.h"

namespace Engine {
	class Sprite;
}

class StartMenu : public Menu {
public:
	StartMenu(Font* font);
	~StartMenu();
	void Draw(Engine::BackBuffer& backBuffer);
	void addElements();
private:
	Engine::Sprite* m_logo;
};