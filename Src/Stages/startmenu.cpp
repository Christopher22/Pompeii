#include "startmenu.h"

#include "mapmenu.h"
#include "aboutmenu.h"
#include "tutorialmenu.h"

#include "Helper/label.h"
#include "Helper/ashparticles.h"

#include "../Engine/sprite.h"
#include "../Engine/backbuffer.h"

#include "../pompeii.h"

StartMenu::StartMenu(Font* font) : Menu(font), m_logo(Pompeii::GetInstance().GetBackbuffer()->CreateSprite("Assets/logo.jpg")) {
	if (m_logo) {
		m_logo->SetX(Pompeii::GetInstance().GetBackbuffer()->GetWidth() - m_logo->GetWidth() - 32);
	}
}

StartMenu::~StartMenu() {
	if (m_logo) {
		delete m_logo;
		m_logo = nullptr;
	}
}

void StartMenu::Draw(Engine::BackBuffer& backBuffer) {
	Menu::Draw(backBuffer);
	if (m_logo) {
		m_logo->Draw(backBuffer);
	}
}

void StartMenu::addElements() {
	this->addElement(new MenuEntry(new Label(GetFont(), "Pompeii", 2.0f), nullptr));

	this->addElement(new MenuEntry(new Label(GetFont(), "New game"), [](Menu*) {
		auto maps(MapMenu::GetAvailableMaps());
		if (maps.size() > 0) {
			auto randomMap = maps.begin();
			std::advance(randomMap, rand() % maps.size());
			Pompeii::GetInstance().StartGame(randomMap->second.c_str());
		}
	}));

	this->addElement(new MenuEntry(new Label(GetFont(), "Load map"), [](Menu*) {
		auto maps(MapMenu::GetAvailableMaps());
		if (maps.size() > 0) {
			auto nextMenu = new MapMenu(Pompeii::GetInstance().GetFont(), maps);
			nextMenu->Initialize(*Pompeii::GetInstance().GetBackbuffer());
			Pompeii::GetInstance().PushStage(nextMenu);
		}
	}));

	this->addElement(new MenuEntry(new Label(GetFont(), "Tutorial"), [](Menu*) {
		auto aboutMenu = new TutorialMenu(Pompeii::GetInstance().GetFont());
		aboutMenu->Initialize(*Pompeii::GetInstance().GetBackbuffer());
		Pompeii::GetInstance().PushStage(aboutMenu);
	}));

	this->addElement(new MenuEntry(new Label(GetFont(), "About"), [](Menu*) {
		auto aboutMenu = new AboutMenu(Pompeii::GetInstance().GetFont());
		aboutMenu->Initialize(*Pompeii::GetInstance().GetBackbuffer());
		Pompeii::GetInstance().PushStage(aboutMenu);
	}));

	this->addElement(new MenuEntry(new Label(GetFont(), "Exit"), [](Menu*) {
		Pompeii::GetInstance().Quit();
	}));
}