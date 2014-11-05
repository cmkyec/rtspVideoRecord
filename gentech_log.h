#ifndef _GENTECH_LOG_H_
#define _GENTECH_LOG_H_

namespace gentech
{

bool logInit(const char* logFilePath);

void logWrite(const char* msg);

void logClose();

}

#endif /* _GENTECH_LOG_H_ */