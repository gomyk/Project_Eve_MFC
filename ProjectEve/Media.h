#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)

extern "C"
{
#ifdef __cplusplus
#define __STDC_CONSTANT_MACROS
#ifdef _STDINT_H
#undef _STDINT_H
#endif

#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavfilter\avfilter.h>
#include <libavfilter\buffersink.h>
#include <libavfilter\buffersrc.h>
#include "RGBImage.h"
#include "MovingImage.h"
#include "Media.h"
#include "Circle.h"

#endif
}

#pragma comment( lib, "avformat.lib" )  
class Media {
private:
	AVInputFormat	*ifmt = NULL;
	AVOutputFormat  *ofmt = NULL;
	
	AVFrame* filtered_frame;

	AVPacket read_pkt;
	AVPacket out_pkt;

	AVFrame* pVFrame;
	AVFrame* pAFrame;

	int nVSI = -1;
	int nASI = -1;
public:
	AVCodecContext* enc_codec_ctx = NULL;
	AVCodecContext* dec_ctx = NULL;
	AVFormatContext *ifmt_ctx = NULL;
	void init_input_format(const char* input_file_path);
	void init_decoder(int flag);
	void init_encoder( int flag, AVCodecID v_codec_id,AVCodecContext* temp_ctx);
	double calc_time_base();
};