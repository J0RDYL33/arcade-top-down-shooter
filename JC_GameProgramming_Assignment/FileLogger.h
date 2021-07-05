#pragma once
#include "SDL.h"
#include <iostream>
#include <ostream>
#include <fstream>
using namespace std;

class FileLogger
{
public:
	FileLogger(const std::string logFilePath);

	~FileLogger(void);

	void GetTime();

	void LoggingFunction(void* userdata, int category, SDL_LogPriority priority, const char* message);

	static void LogCB(void* userdata, int category, SDL_LogPriority priority, const char* message);

private:
	std::ofstream infoLogFile;
	std::ofstream criticalLogFile;
};

