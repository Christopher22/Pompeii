#pragma once

#include "menu.h"

class Match;
class Fire;

namespace Engine {
	class Game;
}

class ResultMenu : public Menu {
public:
	ResultMenu(Font* font, Match* match);
	~ResultMenu();
	void addElements();
	void Draw(Engine::BackBuffer& backBuffer);
	void Process(float deltaTime);
private:
	Match* m_pMatch;
	Fire* m_pFire;
};