#include "stdafx.h"
#include "Media.h"

void Media::init_input_format(const char* input_file_path) {
	AVCodec              *codec = NULL;
	AVCodecContext	         *c = NULL;
	AVFormatContext *p_ifmt_ctx = NULL;

	if ((avformat_open_input(&p_ifmt_ctx, input_file_path, 0, 0)) < 0)
	{
		fprintf(stderr, "Could not open input file '%s'", input_file_path);
	}

	if ((avformat_find_stream_info(p_ifmt_ctx, 0)) < 0)
	{
		fprintf(stderr, "Failed to retrieve input stream information");
	}

	av_dump_format(p_ifmt_ctx, 0, input_file_path, 0);

	///> Find Video Stream
	for (int i = 0; i < p_ifmt_ctx->nb_streams; i++)
	{
		if (nVSI < 0 && p_ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			nVSI = 0;
		}
		else if (nASI < 0 && p_ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			nASI = 1;
		}
	}

	if (nVSI < 0 && nASI < 0)
	{
		av_log(NULL, AV_LOG_ERROR, "No Video & Audio Streams were Found\n");
		return ;
	}
	ifmt_ctx = p_ifmt_ctx;
	return ;
}
void Media::init_decoder(int flag) {
	AVCodecContext *pCtx = ifmt_ctx->streams[nVSI]->codec;

	for (int i = 0; i < ifmt_ctx->nb_streams; i++)
	{
		if (nVSI < 0 && ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			nVSI = 0;
		}
		else if (nASI < 0 && ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			nASI = 1;
		}
	}
	if (nVSI < 0 && nASI < 0)
	{
		av_log(NULL, AV_LOG_ERROR, "No Video & Audio Streams were Found\n");
		return ;
	}

	///> Find Video Decoder
	if (flag == 0) {
		AVCodec   *pVideoCodec = avcodec_find_decoder(ifmt_ctx->streams[nVSI]->codec->codec_id);

		if (pVideoCodec == NULL)
		{
			av_log(NULL, AV_LOG_ERROR, "No Video Decoder was Found\n");
			return ;
		}

		///> Initialize Codec Context as Decoder
		if (avcodec_open2(pCtx, pVideoCodec, NULL) < 0)
		{
			av_log(NULL, AV_LOG_ERROR, "Fail to Initialize Decoder\n");
			return ;
		}
	}

	///> Find Audio Decoder
	else {
		AVCodec *pAudioCodec = avcodec_find_decoder(ifmt_ctx->streams[0]->codec->codec_id);

		if (pAudioCodec == NULL)
		{
			av_log(NULL, AV_LOG_ERROR, "No Audio Decoder was Found\n");
			return ;
		}

		///> Initialize Codec Context as Decoder
		if (avcodec_open2(pCtx, pAudioCodec, NULL) < 0)
		{
			av_log(NULL, AV_LOG_ERROR, "Fail to Initialize Decoder\n");
			return ;
		}
	}
	pCtx->flags |= CODEC_FLAG_LOW_DELAY;
	dec_ctx = pCtx;
}
void Media::init_encoder(int flag,AVCodecID codec_id,AVCodecContext* in_ctx) {
	AVCodec        *codec = NULL;
	AVCodecContext *c_ctx = NULL;

	if (flag == 0) {
		if (nVSI != -1) {
			codec = avcodec_find_encoder(codec_id);
			if (!codec)
			{
				fprintf(stderr, "Codec not found\n");
				return ;
			}

			c_ctx = avcodec_alloc_context3(codec);
			if (!c_ctx)
			{
				fprintf(stderr, "Could not allocate video codec context\n");
				return ;
			}

			// Example
			if (codec_id == AV_CODEC_ID_H264)
				c_ctx->profile = FF_PROFILE_H264_BASELINE;
			c_ctx->width = in_ctx->width;
			c_ctx->height = in_ctx->height;
			c_ctx->pix_fmt = ifmt_ctx->streams[nVSI]->codec->pix_fmt;
			if (codec_id == AV_CODEC_ID_H264) {
				c_ctx->time_base = AVRational{ 1,90000 };
			}
			
			c_ctx->bit_rate = 50000 * 1000;

			if (avcodec_open2(c_ctx, codec, NULL) < 0)
			{
				fprintf(stderr, "Could not open codec\n");
				return ;
			}
		}
	}

	if (flag == 1) {
		if (nASI != -1) {
			codec = avcodec_find_encoder(codec_id);
			if (!codec)
			{
				fprintf(stderr, "Codec not found\n");
				return ;
			}

			c_ctx = avcodec_alloc_context3(codec);
			if (!c_ctx)
			{
				fprintf(stderr, "Could not allocate video codec context\n");
				return ;
			}

			c_ctx->bit_rate = 192 * 1000;
			c_ctx->time_base = { 1,44100 };
			c_ctx->sample_rate = 44100;
			c_ctx->channel_layout = ifmt_ctx->streams[0]->codec->channel_layout;
			c_ctx->channels = ifmt_ctx->streams[0]->codec->channels;
			c_ctx->sample_fmt = ifmt_ctx->streams[0]->codec->sample_fmt;

			// Example

			if (avcodec_open2(c_ctx, codec, NULL) < 0)
			{
				fprintf(stderr, "Could not open codec\n");
				return ;
			}
		}
	}
	enc_codec_ctx = c_ctx;
	return ;
}
double Media::calc_time_base() {
	return double((dec_ctx->time_base.den) / (dec_ctx->time_base.num));
}