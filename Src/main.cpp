// Library includes:
#include <SDL.h>
#include <crtdbg.h>
#include <cassert>

// Local includes:
#include "pompeii.h"
#include "Engine/logmanager.h"

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(506);

	Pompeii& gameInstance = Pompeii::GetInstance();
	if (!gameInstance.Initialise())
	{
		Engine::LogManager::GetInstance().Log("Game initialise failed!");
		return (1);
	}

	while (gameInstance.DoGameLoop())
	{
		// No body.
	}

	Pompeii::DestroyInstance();
	Engine::LogManager::DestroyInstance();

	return (0);
}
