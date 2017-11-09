#include "tutorialmenu.h"

#include "Helper/label.h"

#include <cassert>

#include "../pompeii.h"

TutorialMenu::TutorialMenu(Font* font) : Menu(font) {}

void TutorialMenu::addElements() {
	this->addElement(new MenuEntry(new Label(GetFont(), "Tutorial", 2.0f), nullptr));
	this->addElement(new MenuEntry(new Label(GetFont(), "Movement: D-pad"), nullptr));
	this->addElement(new MenuEntry(new Label(GetFont(), "Selection: A-Button"), nullptr));
	this->addElement(new MenuEntry(new Label(GetFont(), "(Cheat) Extra movement: 'c'"), nullptr));
	this->addElement(new MenuEntry(new Label(GetFont(), "(Cheat) Skip round: 'g'"), nullptr));

	this->addElement(new MenuEntry(new Label(GetFont(), "Back to menu"), [](Menu*) {
		assert(Pompeii::GetInstance().GetStages() == 2);
		Pompeii::GetInstance().PopStage();
	}));
}