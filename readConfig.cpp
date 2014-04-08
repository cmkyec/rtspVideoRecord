#include <fstream>
#include <cstdlib>
#include "json/json.h"
#include "readConfig.h"

namespace gentech
{

static inline bool getJsonRoot(Json::Value& root)
{
	std::ifstream infile("./configFiles/rtspRecordConfig");
        Json::Reader reader;
        return reader.parse(infile, root);
}

bool extractHourAndMinute(const std::string& raw, 
			  int& hour, 
			  int& minute) 
{
	std::size_t colonPos = raw.find(":");
	if (colonPos == std::string::npos) return false;
	if (colonPos == raw.length() - 1) return false;
	// 格式为 08:03，小时和分各占2个位置
	if (colonPos != raw.length() / 2) return false;
	std::string strHour = raw.substr(0, colonPos);
	std::string strMinute = raw.substr(colonPos + 1, raw.length());
	hour = atoi(strHour.c_str());
	minute = atoi(strMinute.c_str());
	return (hour >= 0 && hour <= 24 &&
		minute >=0 && minute <= 60);
}

bool extractRecordBeginTimeInfo(struct recordBeginTimeInfo& info)
{
        Json::Value root;
	if (!getJsonRoot(root)) return false;

	const Json::Value recordBeginTimeJson = root["recordBeginTime"];
	if (recordBeginTimeJson.size() == 0) return false;
	for (unsigned int i = 0; i < recordBeginTimeJson.size(); ++i) {
		std::string raw = recordBeginTimeJson[i].asString();
		int hour = 0, minute = 0;
		if (!extractHourAndMinute(raw, hour, minute)) return false;
		info.m_hours.push_back(hour);
		info.m_minutes.push_back(minute);
	}
	return true;
}

bool extractRecordLength(int& length)
{
	Json::Value root;
	if (!getJsonRoot(root)) return false;

	const Json::Value recordLengthJson = root["recordLength"];
	length = recordLengthJson.asInt();
	return length > 0;
}

bool extractRecordNumberEachTime(int& recordNumberEachTime)
{	
	Json::Value root;
	if (!getJsonRoot(root)) return false;
	const Json::Value recordNumberEachTimeJson = root["recordNumberEachTime"];
	recordNumberEachTime = recordNumberEachTimeJson.asInt();
	return recordNumberEachTime > 0;
}

bool extractRtspSource(std::map<std::string, std::string>& rtspSource)
{
	Json::Value root;
	if (!getJsonRoot(root)) return false;
	const Json::Value rtspSourceJson = root["rtspSource"];
	if (rtspSourceJson.size() == 0) return false;
	Json::Value::Members sourceNames = rtspSourceJson.getMemberNames();
	for (std::size_t i = 0; i < sourceNames.size(); ++i) {
		std::string sourceName = sourceNames[i];
		std::string sourcePath = rtspSourceJson[sourceName].asString();
		rtspSource.insert(std::pair<std::string, std::string>(sourceName, sourcePath));
	}
	return true;
}

}
