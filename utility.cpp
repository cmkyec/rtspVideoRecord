#include "utility.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <unistd.h>

namespace gentech
{

bool generateRandomNumbers(int count, int min, int max,
		           std::vector<int>& randNumbers)
{
	if (count > (max -min)) return false;

	srand(time(NULL));
	randNumbers.clear();

	for (;;) {
		int r = rand() % (max - min) + min;
		std::size_t i = 0;
		for (i = 0; i < randNumbers.size(); ++i) {
			if (randNumbers[i] == r) break;
		}
		if (randNumbers.size() == i) {
			randNumbers.push_back(r);
		}
		if (randNumbers.size() == count) break;
	}
	return true;
}

bool getRandomRtspSource(std::map<std::string, std::string>& randRtspScource)
{
	std::map<std::string, std::string> totalRtspSource;
	if (!extractRtspSource(totalRtspSource)) return false;
	int recordNumberEachTime = -1;
	if (!extractRecordNumberEachTime(recordNumberEachTime)) return false;

	int min = 0, max = totalRtspSource.size();
	std::vector<int> randNumbers;
	if (!generateRandomNumbers(recordNumberEachTime, min, max, randNumbers)) {
			return false;
	}
	std::sort(randNumbers.begin(), randNumbers.end());

	std::map<std::string, std::string>::iterator it = totalRtspSource.begin();
	for (int i = 0; it != totalRtspSource.end(); it++, i++) {
		std::size_t j = 0;
		for (; j < randNumbers.size(); ++j) {
			if (i == randNumbers[j]) break;
		}
		if (j != randNumbers.size()) {
			randRtspScource.insert(*it);
		}
	}
	return true;
}

void generateFileName(std::string& pureName, std::string& finalName)
{
	time_t rawtime;
	time(&rawtime);
	struct tm* timeinfo;
	timeinfo = localtime(&rawtime);

	char tmp[256];
	memset(tmp, 0, sizeof(tmp));
	strftime(tmp, 256, "%Y-%m-%d_%H-%M-%S", timeinfo);
	finalName = std::string(tmp) + "_" + pureName + ".avi";

	return;
}

bool shouldStartRecord()
{
	struct recordBeginTimeInfo info;
	if (!extractRecordBeginTimeInfo(info)) return false;

	time_t curTime;
	time(&curTime);
	struct tm* curTimeInfo;
	curTimeInfo = localtime(&curTime);
	int curHour = curTimeInfo->tm_hour;
	int curMinute = curTimeInfo->tm_min;

	bool ret = false;
	for (std::size_t i = 0; i < info.m_hours.size(); ++i) {
		if (curHour == info.m_hours[i] && 
		    (std::abs(curMinute - info.m_minutes[i]) < 2) ) {
			ret = true;
			break;
		}
	}
	return ret;
}

bool shouldEndRecord(time_t start)
{
	int recordLength = -1;
	if (!extractRecordLength(recordLength)) return false;
	time_t now;
	time(&now);
	return difftime(now, start) > (recordLength * 60 + 60);
}

}
