#include "rtspVideoRecord.h"
#include <string.h>

namespace gentech
{

CRtspVideoRecord::CRtspVideoRecord()
{
	m_pFormatCtx = NULL;
	m_pCodecCtx = NULL;
	m_pCodec = NULL;
	m_videoStreamIndex = -1;

	memset(m_errorMsg, 0, sizeof(m_errorMsg));

	av_register_all();
}

CRtspVideoRecord::~CRtspVideoRecord()
{
	close();
}

int CRtspVideoRecord::open(const char* pStreamPath)
{
	int ret = -1;

	// open input rtsp stream and read the header.
	ret = avformat_open_input(&m_pFormatCtx, pStreamPath, NULL, NULL);
	if (ret != 0) {
		memset(m_errorMsg, 0, sizeof(m_errorMsg));
		av_strerror(ret, m_errorMsg, sizeof(m_errorMsg));
		close();
		return ret;
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
		memset(m_errorMsg, 0, sizeof(m_errorMsg));
		sprintf(m_errorMsg, "can not find video stream.");
		close();
		return -1;
	}
	
	// open the video decoder
	m_pCodecCtx = m_pFormatCtx->streams[m_videoStreamIndex]->codec;
	m_pCodec = avcodec_find_decoder(m_pCodecCtx->codec_id);
	if (m_pCodec == NULL) {
		memset(m_errorMsg, 0, sizeof(m_errorMsg));
		sprintf(m_errorMsg, "can not find a registerted decoder for codec id: %d.",
			m_pCodecCtx->codec_id);
		close();
		return -1;
	}
	ret = avcodec_open2(m_pCodecCtx, m_pCodec, NULL);
	if (ret < 0) {
		memset(m_errorMsg, 0, sizeof(m_errorMsg));
		av_strerror(ret, m_errorMsg, sizeof(m_errorMsg));
		close();
		return ret;
	}

	return 0;
}

int CRtspVideoRecord::write(FILE* pFile)
{
	AVPacket pkt;
	int ret = -1;
	ret = av_read_frame(m_pFormatCtx, &pkt);
	if (ret < 0) {
		return -1;
	}
	fwrite(pkt.data, sizeof(uint8_t), pkt.size, pFile);
	av_free_packet(&pkt);
	return 0;
}

int CRtspVideoRecord::close()
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
	memset(m_errorMsg, 0, sizeof(m_errorMsg));
	m_videoStreamIndex = -1;
}

}
