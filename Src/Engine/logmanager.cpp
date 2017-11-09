// This include:
#include "logmanager.h"

// Library includes:
#include <Windows.h>

namespace Engine
{
	// Static Members:
	LogManager* LogManager::sm_pInstance = nullptr;

	LogManager&
		LogManager::GetInstance()
	{
		if (sm_pInstance == nullptr)
		{
			sm_pInstance = new LogManager();
		}

		return (*sm_pInstance);
	}

	void
		LogManager::DestroyInstance()
	{
		delete sm_pInstance;
		sm_pInstance = nullptr;
	}

	LogManager::LogManager()
	{

	}

	LogManager::~LogManager()
	{

	}

	void
		LogManager::Log(const char* pcMessage)
	{
		OutputDebugStringA(pcMessage);
		OutputDebugStringA("\n");
	}
}