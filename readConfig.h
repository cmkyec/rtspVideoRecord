#ifndef _READ_CONFIG_H_
#define _READ_CONFIG_H_

#include <string>
#include <vector>
#include <map>

namespace gentech
{

/**
 * !!!!!!Note:
 * 配置文件必须存储在文件所在目录的 configFiles 文件夹中，
 * 且必须命名为 rtspRecordConfig
 */
// 每天开始录像的时间点， m_hours的长度应该和m_minutes的长度相同
struct recordBeginTimeInfo
{
	std::vector<int> m_hours;
	std::vector<int> m_minutes;
};

// 从配置文件中读取每天开始录像的时间点
bool extractRecordBeginTimeInfo(struct recordBeginTimeInfo& info);

// 从配置文件中，读取每段录像的长度
bool extractRecordLength(int& length);

// 从配置文件中，读取每个时间点随机录制的视频数量
bool extractRecordNumberEachTime(int& recordNumberEachTime);

// 从配置文件中，读取录像的视频源名称和视频源地址
bool extractRtspSource(std::map<std::string, std::string>& rtspSource);

}


#endif /* _READ_CONFIG_H_ */
