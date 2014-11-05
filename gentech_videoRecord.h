#ifndef _VIDEO_RECORD_H_
#define _VIDEO_RECORD_H_

#define __STDC_CONSTANT_MACROS
extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}
#include <stdio.h>

namespace gentech
{

class CVideoRecord
{
public:
	CVideoRecord();

	~CVideoRecord();

	int open(const char* pStreamPath);

	int write(FILE* pFile);

	void close();
private:
	AVFormatContext* m_pFormatCtx;
	AVCodecContext* m_pCodecCtx;
	AVCodec* m_pCodec;
	int m_videoStreamIndex;
};

}

#endif /* _VIDEO_RECORD_H_ */
