// COMP710 GP 2D Framework 2017
#ifndef __LOGMANAGER_H__
#define __LOGMANAGER_H__

#include <cstdio>

namespace Engine
{
	class LogManager
	{
		//Member Methods:
	public:
		static LogManager& GetInstance();
		static void DestroyInstance();
		~LogManager();

		void Log(const char* pcMessage);
	
		template<typename ... Args>
		void Log(const char* format, Args ... args)
		{
			// Adapted from https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
			auto size = _snprintf(nullptr, 0, format, args ...) + 1; // Extra space for '\0'
			char* output = new char[size];
			_snprintf(output, size, format, args ...);
			Log(output);
			delete[] output;
			output = nullptr;
		}

	private:
		LogManager();
		LogManager(const LogManager& logManager);
		LogManager& operator=(const LogManager& logManager);

		//Member Data:
	public:

	protected:
		static LogManager* sm_pInstance;

	private:

	};
}

#endif //__LOGMANAGER_H__
