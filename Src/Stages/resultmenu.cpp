#include "resultmenu.h"

#include "match.h"

#include "Helper/label.h"
#include "Helper/fire.h"

#include "../pompeii.h"

#include <cassert>
#include <iomanip>
#include <sstream>

ResultMenu::ResultMenu(Font* font, Match* match) : Menu(font), m_pMatch(match), m_pFire(new Fire()) {
	assert(match);

	auto backbuffer = Pompeii::GetInstance().GetBackbuffer();
	m_pFire->SetPosition(float(backbuffer->GetWidth() / 2), float(backbuffer->GetWidth() / 2 + backbuffer->GetWidth() / 5 + 20));
}

ResultMenu::~ResultMenu() {
	delete m_pFire;
}

void ResultMenu::addElements() {
	this->addElement(new MenuEntry(new Label(GetFont(), m_pMatch->GetStatus() == Match::Status::Win ? "You win!" : "You lose!", 2.0f), nullptr));
	if (m_pMatch->GetStatus() == Match::Status::Win) {
		std::ostringstream ss;
		ss << "Your score: " << std::setw(2) << std::setfill('0') << m_pMatch->GetRound();
		this->addElement(new MenuEntry(new Label(GetFont(), ss.str().c_str()), nullptr));
	}

	this->addElement(new MenuEntry(new Label(GetFont(), "Replay"), [](Menu*) {
		assert(Pompeii::GetInstance().GetStages() == 3);
		Pompeii::GetInstance().PopStage();

		Match* match = dynamic_cast<Match*>(Pompeii::GetInstance().GetStage());
		assert(match);

		auto path = match->GetPath();
		Pompeii::GetInstance().PopStage();

		Pompeii::GetInstance().PushStage(Match::LoadMatch(Pompeii::GetInstance().GetStage()->GetFont(), path.c_str()));
	}));

	this->addElement(new MenuEntry(new Label(GetFont(), "Back to menu"), [](Menu*) {
		assert(Pompeii::GetInstance().GetStages() == 3);
		Pompeii::GetInstance().PopStage();
		Pompeii::GetInstance().PopStage();
	}));
}

void ResultMenu::Draw(Engine::BackBuffer& backBuffer) {
	m_pFire->Draw(backBuffer);
	Menu::Draw(backBuffer);
}

void ResultMenu::Process(float deltaTime) {
	m_pFire->Process(&Pompeii::GetInstance(), deltaTime);
}