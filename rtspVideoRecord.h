#ifndef _RTSP_VIDEO_RECORD_H_
#define _RTSP_VIDEO_RECORD_H_

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

class CRtspVideoRecord
{
public:
	CRtspVideoRecord();

	~CRtspVideoRecord();

	int open(const char* pStreamPath);

	int write(FILE* pFile);

	int close();
private:
	AVFormatContext* m_pFormatCtx;
	AVCodecContext* m_pCodecCtx;
	AVCodec* m_pCodec;
	int m_videoStreamIndex;
	char m_errorMsg[256];
};

}

#endif /* _RTSP_VIDEO_RECORD_H_ */
