// This include:
#include "inputhandler.h"

// Local includes:
#include "game.h"
#include "logmanager.h"

// Library includes:
#include <cassert>

namespace Engine
{
	InputHandler::InputHandler()
		: m_pGameController(0)
	{

	}

	InputHandler::~InputHandler()
	{
		if (m_pGameController)
		{
			//SDL_JoystickClose(m_pGameController);
			m_pGameController = 0;
		}
	}

	bool
		InputHandler::Initialise()
	{
		int numControllesr = SDL_NumJoysticks();

		m_pGameController = SDL_JoystickOpen(0);

		if (!m_pGameController)
		{
			LogManager::GetInstance().Log("No controller detected!");
		}

		return (true);
	}

	void
		InputHandler::ProcessInput(Game& game)
	{
		::SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				game.Quit();
			}
			else {
				game.Input(&e);
			}
		}
	}
}