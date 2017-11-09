#include "pompeii.h"

#include "Stages/stage.h"
#include "Stages/match.h"
#include "Stages/startmenu.h"

#include "Stages/Helper/font.h"
#include "Stages/Helper/ashparticles.h"

#include "Engine/backbuffer.h"
#include "Engine/texturemanager.h"
#include "Engine/logmanager.h"

#include "fmod.hpp" 

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <ctime>

Pompeii* Pompeii::sm_pInstance = nullptr;

Pompeii& Pompeii::GetInstance() {
	if (sm_pInstance == nullptr) {
		sm_pInstance = new Pompeii();
	}

	return (*sm_pInstance);
}

void Pompeii::DestroyInstance() {
	delete sm_pInstance;
	sm_pInstance = nullptr;
}

Pompeii::Pompeii() : Engine::Game(), m_stages(), m_pAudio(nullptr), m_pBackgroundMusic(nullptr), m_pFont(nullptr), m_pAsh(new AshParticles()) {
	srand(unsigned int(time(NULL)));
}

Pompeii::~Pompeii() {
	if (m_pBackgroundMusic) {
		m_pBackgroundMusic->release();
		m_pBackgroundMusic = nullptr;
	}

	if (m_pAudio) {
		m_pAudio->release();
		m_pAudio = nullptr;
	}
	
	if (m_pFont) {
		delete m_pFont;
		m_pFont = nullptr;
	}

	delete m_pAsh;

	while (m_stages.size() > 0) {
		delete m_stages.top();
		m_stages.top() = nullptr;
		m_stages.pop();
	}
}

void Pompeii::Input(SDL_Event* input) {
	m_stages.top()->Input(input);
}

void Pompeii::Process(float deltaTime) {
	m_pAudio->update();
	m_pAsh->Process(this, deltaTime);

	m_stages.top()->Process(deltaTime);
}

void Pompeii::Draw(Engine::BackBuffer& backBuffer) {
	m_stages.top()->Draw(backBuffer);
	m_pAsh->Draw(backBuffer);
}

bool Pompeii::Initialise() {
	if (!Game::Initialise("Pompeii", 640, 480)) {
		return false;
	}

	if (FMOD::System_Create(&m_pAudio) != FMOD_OK || m_pAudio->init(512, FMOD_INIT_NORMAL, 0) != FMOD_OK) {
		Engine::LogManager::GetInstance().Log("Initialisation of FMOD failed!");
		return false;
	}

	if (m_pAudio->createSound("Assets\\Sounds\\background.mp3", FMOD_LOOP_NORMAL ^ FMOD_2D, nullptr, &m_pBackgroundMusic) != FMOD_OK) {
		Engine::LogManager::GetInstance().Log("Loading of background audio failed!");
		return false;
	} else {
		m_pAudio->playSound(m_pBackgroundMusic, nullptr, false, nullptr);
	}

	Engine::Texture* fontTexture = this->GetBackbuffer()->GetTextureManager()->GetTexture("Assets\\font.png");
	if (!fontTexture) {
		Engine::LogManager::GetInstance().Log("Loading of font failed");
		return false;
	}
	m_pFont = new Font(*fontTexture, 56, 46, 0, 0, 22, 13);

	StartMenu* start = new StartMenu(m_pFont);
	start->Initialize(*this->GetBackbuffer());
	m_stages.push(start);
	return true;	
}

bool Pompeii::StartGame(const char* map) {
	auto game = Match::LoadMatch(m_pFont, map);
	if (game) {
		m_stages.emplace(game);
	}
	return (game != nullptr);
}

bool Pompeii::PopStage() {
	if (m_stages.size() > 1) {
		delete m_stages.top();
		m_stages.top() = nullptr;
		m_stages.pop();
		return true;
	} else {
		return false;
	}
}

void Pompeii::PushStage(Stage* stage) {
	assert(stage);
	m_stages.emplace(stage);
}

Stage* Pompeii::GetStage() const {
	assert(m_stages.top());
	return m_stages.top();
}

unsigned short Pompeii::GetStages() const {
	return m_stages.size();
}

Font* Pompeii::GetFont() const {
	return m_pFont;
}