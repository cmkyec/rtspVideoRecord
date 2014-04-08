#include <iostream>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include "rtspVideoRecord.h"
#include "readConfig.h"
#include "utility.h"

//pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;


/*
int main()
{
	const char* pStreamPath = "rtsp://218.204.223.237:554/live/1/524618711AAE34ED/e4i04zzojmeaix01.sdp";
	//const char* pStreamPath = "rtsp://218.204.223.237:554/live/1/1A0D5EB67E333A01/q210pponns64dzqm.sdp";
	gentech::CRtspVideoRecord record;
	if (record.open(pStreamPath) != 0) {
		std::cerr<<"can not open the stream"<<std::endl;
		return 1;
	}

	FILE* pFile = fopen("./test.avi", "wb");
	for (int i = 0; i < 500; i++) {
		if (record.write(pFile) != 0) {
			std::cout<<"failed."<<std::endl;
		} else {
			std::cout<<i<<std::endl;
		}
		sleep(1);
	}
	fclose(pFile);
	return 0;
}
*/

struct videoRecordParam
{
	char m_recordVideoName[256];
	int m_recordLength;
	char m_sourcePath[256];

	videoRecordParam()
	{
		memset(m_recordVideoName, 0, sizeof(m_recordVideoName));
		m_recordLength = 0;
		memset(m_sourcePath, 0, sizeof(m_sourcePath));
	}
};

void* videoRecord(void* args)
{
	const struct videoRecordParam* param = (const struct videoRecordParam*)args;
	gentech::CRtspVideoRecord record;
	std::cout<<"11:"<<param->m_recordVideoName<<std::endl;
	std::cout<<"22:"<<param->m_sourcePath<<std::endl;
	std::cout<<"33:"<<param->m_recordLength<<std::endl;

	if (record.open(param->m_sourcePath) != 0) {
		std::cerr<<"can not open the stream"<<std::endl;
		return NULL;
	}
	FILE* pFile = fopen(param->m_recordVideoName, "wb");
	unsigned int frameCount = param->m_recordLength * 23 * 60; // 23帧每秒
	for (unsigned int i = 0; i < frameCount; ++i) {
		record.write(pFile);
	}
	fclose(pFile);
	return NULL;
}

void createThreads(std::vector<pthread_t>& threadIDs, 
		   std::vector<struct videoRecordParam>& threadParams) 
{
	threadIDs.clear();
	threadParams.clear();

	std::map<std::string, std::string> randRtspSource;
	if (!gentech::getRandomRtspSource(randRtspSource)) return;
	int recordLength = 0;
	if (!gentech::extractRecordLength(recordLength)) return;

	std::map<std::string, std::string>::iterator it;
	for (it = randRtspSource.begin(); it != randRtspSource.end(); it++) {
		std::string pureName(it->first);
		std::string finalName;
		gentech::generateFileName(pureName, finalName);
		struct videoRecordParam param;
		sprintf(param.m_recordVideoName, "%s", finalName.c_str());
		sprintf(param.m_sourcePath, "%s", (it->second).c_str());
		param.m_recordLength = recordLength;
		threadParams.push_back(param);
		pthread_t threadid;
		if (pthread_create(&threadid, NULL, videoRecord, 
				   (void*)(&(threadParams[threadParams.size() - 1]))) != 0) {
			continue;
		}
		pthread_detach(threadid);
		threadIDs.push_back(threadid);
	}
}

void destroyThreads(std::vector<pthread_t>& threadIDs,
		    std::vector<struct videoRecordParam>& threadParams)
{
	for (std::size_t i = 0; i < threadIDs.size(); ++i) {
		if (pthread_kill(threadIDs[i], 0) != ESRCH) {
			pthread_cancel(threadIDs[i]);
		}
		threadIDs[i] = 0;
	}
	threadIDs.clear();
	threadParams.clear();	
}

int main()
{
	std::vector<pthread_t> ids;
	std::vector<struct videoRecordParam> params;
	time_t createdTime;
	while (true) {
		if (gentech::shouldStartRecord() && ids.size() == 0) {
			createThreads(ids, params);
			time(&createdTime);
			std::cout<<"create thread."<<std::endl;
		}
		if (gentech::shouldEndRecord(createdTime) && ids.size() != 0) {
			destroyThreads(ids, params);
			std::cout<<"destroy thread."<<std::endl;
		}
		sleep(20);
	}
	return 0;
}

