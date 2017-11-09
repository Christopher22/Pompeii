#include "aboutmenu.h"

#include "Helper/label.h"

#include <cassert>

#include "../pompeii.h"

AboutMenu::AboutMenu(Font* font) : Menu(font) {}

void AboutMenu::addElements() {
	this->addElement(new MenuEntry(new Label(GetFont(), "Pompeii by Christopher Gundler", 0.75f), nullptr));
	this->addElement(new MenuEntry(new Label(GetFont(), "Engine by Steffan Hooper", 0.75f), nullptr));
	this->addElement(new MenuEntry(new Label(GetFont(), "Background by Soar49", 0.75f), nullptr));
	this->addElement(new MenuEntry(new Label(GetFont(), "Characters by the Open Pixel Project", 0.75f), nullptr));
	this->addElement(new MenuEntry(new Label(GetFont(), "Music by Pierluigi Molinari", 0.75f), nullptr));
	this->addElement(new MenuEntry(new Label(GetFont(), "Font 'Cinzel' by Natanael Gama", 0.75f), nullptr));

	this->addElement(new MenuEntry(new Label(GetFont(), "Back to menu", 0.75f), [](Menu*) {
		assert(Pompeii::GetInstance().GetStages() == 2);
		Pompeii::GetInstance().PopStage();
	}));
}