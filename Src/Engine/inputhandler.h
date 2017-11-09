// COMP710 GP 2D Framework 2017
#ifndef __INPUTHANDER_H__
#define __INPUTHANDER_H__

#include <SDL.h>

namespace Engine
{
	// Forward Declarations:
	class Game;

	class InputHandler
	{
		//Member Methods:
	public:
		InputHandler();
		~InputHandler();

		bool Initialise();
		void ProcessInput(Game& game);
		
	private:
		InputHandler(const InputHandler& inputHandler);
		InputHandler& operator=(const InputHandler& inputHandler);

		//Member Data:
	public:

	protected:
		SDL_Joystick* m_pGameController;

	private:

	};
}

#endif // __INPUTHANDER_H__
