// This includes:
#include "game.h"

// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <cstdio>

namespace Engine
{
	Game::Game()
		: m_pBackBuffer(0)
		, m_pInputHandler(0)
		, m_looping(true)
		, m_executionTime(0)
		, m_elapsedSeconds(0)
		, m_frameCount(0)
		, m_FPS(0)
		, m_numUpdates(0)
		, m_lastTime(0)
		, m_lag(0)
	{

	}

	Game::~Game()
	{
		delete m_pBackBuffer;
		m_pBackBuffer = nullptr;

		delete m_pInputHandler;
		m_pInputHandler = nullptr;
	}

	bool
		Game::Initialise(const char* title, unsigned short width, unsigned short height)
	{
		m_pBackBuffer = new BackBuffer();
		if (!m_pBackBuffer->Initialise(title, width, height))
		{
			LogManager::GetInstance().Log("BackBuffer Init Fail!");
			return (false);
		}

		m_pInputHandler = new InputHandler();
		if (!m_pInputHandler->Initialise())
		{
			LogManager::GetInstance().Log("InputHandler Init Fail!");
			return (false);
		}

		m_lastTime = SDL_GetTicks();
		m_lag = 0.0f;

		return (true);
	}

	bool
		Game::DoGameLoop()
	{
		const float stepSize = 1.0f / 60.0f;

		assert(m_pInputHandler);
		m_pInputHandler->ProcessInput(*this);

		if (m_looping)
		{
			int current = SDL_GetTicks();
			float deltaTime = (current - m_lastTime) / 1000.0f;
			m_lastTime = current;

			m_executionTime += deltaTime;

			m_lag += deltaTime;

			while (m_lag >= stepSize)
			{
				// Process
				m_elapsedSeconds += deltaTime;
				if (m_elapsedSeconds > 1)
				{
					m_elapsedSeconds -= 1;
					m_FPS = m_frameCount;
					m_frameCount = 0;
				}
				Process(stepSize);

				m_lag -= stepSize;

				++m_numUpdates;
			}

			++m_frameCount;

			// Draw
			m_pBackBuffer->Clear();
			Draw(*m_pBackBuffer);
			m_pBackBuffer->Present();
		}

		//	SDL_Delay(1);

		return (m_looping);
	}

	void
		Game::Quit()
	{
		m_looping = false;
	}

	BackBuffer* Game::GetBackbuffer() const {
		return m_pBackBuffer;
	}
}