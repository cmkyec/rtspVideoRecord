#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <vector>
#include <map>
#include <string>
#include <time.h>
#include "readConfig.h"
#include "rtspVideoRecord.h"

#define GENTECH_FUNC_SUCCEED (0)
#define GENTECH_FUNC_FAILED (-1)

namespace gentech
{

// 在[min, max)范围内，随机挑选count个不同的数, count 必须不大于 (max - min)
// 生成的结果储存在 randNumbers 中，
// 成功返回true, 失败返回false
bool generateRandomNumbers(int count, int min, int max,
		           std::vector<int>& randNumbers);

// 每个时间点，在配置文件所有视频源中，随机挑选指定数目的视频源（配置文件中已经指定）
// 进行录像
bool getRandomRtspSource(std::map<std::string, std::string>& randRtspScource);

// 在pureName前加上当前日期作为前缀，作为最终的文件存储名
// 例如pureName为"K100",
void generateFileName(std::string& pureName, std::string& finalName);

// 根据配置文件中的每天录像的时间点，判断当前是否要开始录像
bool shouldStartRecord();

bool shouldEndRecord(time_t start);

}

#endif /* _UTILITY_H_ */

