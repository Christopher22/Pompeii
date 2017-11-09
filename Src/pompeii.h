#pragma once

#include "Engine/game.h"
#include "Engine/backbuffer.h"

#include "World/types.h"

#include <stack>

namespace Engine {
	class BackBuffer;
}

namespace FMOD {
	class System;
	class Sound;
}

class Stage;
class Font;
class AshParticles;

class Pompeii : public Engine::Game {
public:
	static Pompeii& GetInstance();
	static void DestroyInstance();

	~Pompeii();
	bool Initialise();
	void Input(SDL_Event* input);

	bool StartGame(const char* map);
	Font* GetFont() const;

	void PushStage(Stage* stage);
	bool PopStage();
	Stage* GetStage() const;
	unsigned short GetStages() const;

protected:
	void Process(float deltaTime);
	void Draw(Engine::BackBuffer& backBuffer);
	
private:
	Pompeii();

	static Pompeii* sm_pInstance;

	AshParticles* m_pAsh;
	Font* m_pFont;
	std::stack<Stage*> m_stages;
	FMOD::System* m_pAudio;
	FMOD::Sound* m_pBackgroundMusic;
};

