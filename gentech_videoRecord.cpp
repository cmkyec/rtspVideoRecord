#include "gentech_videoRecord.h"
#include "gentech_log.h"
#include "gentech_utility.h"
#include <string.h>

namespace gentech
{

CVideoRecord::CVideoRecord()
{
	m_pFormatCtx = NULL;
	m_pCodecCtx = NULL;
	m_pCodec = NULL;
	m_videoStreamIndex = -1;

	av_register_all();
	avformat_network_init();
}

CVideoRecord::~CVideoRecord()
{
	close();
}

int CVideoRecord::open(const char* pStreamPath)
{
	int ret = -1;
	char averrmsg[256] = { 0 }, loginfo[500] = { 0 };
	// open input stream and read the header.
	ret = avformat_open_input(&m_pFormatCtx, pStreamPath, NULL, NULL);
	if (ret != 0) {
		av_strerror(ret, averrmsg, sizeof(averrmsg));
		snprintf(loginfo, sizeof(loginfo), "avformat_open_input failed, stream path: %s, errmsg: %s.\n", pStreamPath, averrmsg);
		goto __failed;	
	}

	// find video stream index
	m_videoStreamIndex = -1;
	for (unsigned int i = 0; i < m_pFormatCtx->nb_streams; ++i) {
		if (m_pFormatCtx->streams[i]->codec->codec_type == CODEC_TYPE_VIDEO) {
			m_videoStreamIndex = i;
			break;
		}
	}
	if (m_videoStreamIndex == -1) {
		memset(loginfo, 0, sizeof(loginfo));
		snprintf(loginfo, sizeof(loginfo), "can not find video stream.\n");
		goto __failed;
	}
	
	// open the video decoder
	m_pCodecCtx = m_pFormatCtx->streams[m_videoStreamIndex]->codec;
	m_pCodec = avcodec_find_decoder(m_pCodecCtx->codec_id);
	if (m_pCodec == NULL) {
		memset(loginfo, 0, sizeof(loginfo));
		sprintf(loginfo, "can not find a registerted decoder for codec id: %d.\n",
			m_pCodecCtx->codec_id);
		goto __failed;
	}
	ret = avcodec_open2(m_pCodecCtx, m_pCodec, NULL);
	if (ret < 0) {
		memset(averrmsg, 0, sizeof(averrmsg));
		av_strerror(ret, averrmsg, sizeof(averrmsg));
		memset(loginfo, 0, sizeof(loginfo));
		snprintf(loginfo, sizeof(loginfo), "avcodec_open2 failed, errmsg: %s.\n", averrmsg);
		goto __failed;
	}
	return GENTECH_FUNC_SUCCEED;
__failed:
	gentech::logWrite(loginfo);
	close();
	return GENTECH_FUNC_FAILED;
}

int CVideoRecord::write(FILE* pFile)
{
	AVPacket pkt;
	av_init_packet(&pkt);
	int ret = -1;
	ret = av_read_frame(m_pFormatCtx, &pkt);
	if (ret < 0) {
		char averrmsg[256] = { 0 };
		av_strerror(ret, averrmsg, sizeof(averrmsg));
		char loginfo[500] = { 0 };
		snprintf(loginfo, sizeof(loginfo), "av_read_frame failed, errmsg: %s.\n", averrmsg);
		gentech::logWrite(loginfo);
		return GENTECH_FUNC_FAILED;
	}
	fwrite(pkt.data, sizeof(uint8_t), pkt.size, pFile);
	av_free_packet(&pkt);
	return GENTECH_FUNC_SUCCEED;
}

void CVideoRecord::close()
{
	if (m_pCodecCtx) {
		avcodec_close(m_pCodecCtx);
		m_pCodec = NULL;
		m_pCodecCtx = NULL;
	}
	if (m_pFormatCtx) {
		av_close_input_file(m_pFormatCtx);
		m_pFormatCtx = NULL;
	}
	m_videoStreamIndex = -1;
}

}
