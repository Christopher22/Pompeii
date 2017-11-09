// COMP710 GP 2D Framework 2017
#ifndef __GAME_H__
#define __GAME_H__

union SDL_Event;

namespace Engine
{
	// Forward Declarations
	class BackBuffer;
	class InputHandler;
	class Sprite;

	class Game
	{
		//Member Methods:
	public:
		virtual ~Game();

		virtual bool Initialise(const char* title, unsigned short width, unsigned short height);
		bool DoGameLoop();
		void Quit();
		BackBuffer* GetBackbuffer() const;

		virtual void Input(SDL_Event* input) = 0;

	protected:
		Game();
		virtual void Process(float deltaTime) = 0;
		virtual void Draw(BackBuffer& backBuffer) = 0;

	private:
		Game(const Game& game);
		Game& operator=(const Game& game);

	protected:
		InputHandler* m_pInputHandler;

	private:
		BackBuffer* m_pBackBuffer;
		bool m_looping;

		// Simulation Counters:
		float m_elapsedSeconds;
		float m_lag;
		float m_executionTime;
		int m_lastTime;
		int m_frameCount;
		int m_FPS;
		int m_numUpdates;
		bool m_drawDebugInfo;
	};
}

#endif // __GAME_H__
