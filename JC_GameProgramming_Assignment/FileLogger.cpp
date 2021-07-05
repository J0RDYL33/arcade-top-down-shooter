#include "FileLogger.h"

FileLogger::FileLogger(const std::string logfilename)
{
	infoLogFile.open(logfilename.c_str());

	if (!infoLogFile.is_open())
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Couldn't open file");

	criticalLogFile.open("log-critical.txt");

	if (!criticalLogFile.is_open())
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Couldn't open file");

	SDL_LogSetOutputFunction(&FileLogger::LogCB, (void*)this);
}

FileLogger::~FileLogger()
{
	infoLogFile.close();
}

void FileLogger::LoggingFunction(void* userdata, int category, SDL_LogPriority priority, const char* message)
{
	if (priority == 3)
	{
		GetTime();
		cout << message << endl;
		infoLogFile << message << endl << endl;
	}
	else if (priority == SDL_LOG_PRIORITY_CRITICAL)
	{
		GetTime();
		cout << message << endl;
		criticalLogFile << message << endl << endl;
	}
}

void FileLogger::LogCB(void* userdata, int category, SDL_LogPriority priority, const char* message)
{
	FileLogger* ptr = (FileLogger*)userdata;
	if (ptr)
	{
		ptr->LoggingFunction(userdata, category, priority, message);
	}
}

void FileLogger::GetTime()
{
	time_t rawtime;
	struct tm* timeInfo;
	char buffer[10];
	time(&rawtime);
	timeInfo = localtime(&rawtime);
	strftime(buffer, 10, "%X: ", timeInfo);
	puts(buffer);
}