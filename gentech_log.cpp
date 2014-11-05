#include "gentech_log.h"
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>

using namespace gentech;

static std::ofstream g_logFile;

static std::string getTime()
{
	time_t t = time(NULL);
	struct tm* currTime = localtime(&t);
	static char strTime[20] = { 0 };
	sprintf(strTime, "%04d/%02d/%02d %02d:%02d:%02d",
		currTime->tm_year + 1900, currTime->tm_mon + 1, currTime->tm_mday,
		currTime->tm_hour, currTime->tm_min, currTime->tm_sec);
	return std::string(strTime);
}

bool gentech::logInit(const char* logFilePath)
{
	g_logFile.open(logFilePath, std::ios::app);
	if (!g_logFile.is_open()) return false;
	return true;
}

void gentech::logClose()
{
	g_logFile.close();
}

void gentech::logWrite(const char* msg)
{
	if (!g_logFile.is_open()) return;
	std::string t = getTime();
	std::string log = t + " " + msg;
	g_logFile << log;
}
