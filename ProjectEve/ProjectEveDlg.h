
// ProjectEveDlg.h : 헤더 파일
//

#pragma once
///> Include FFMpeg
#define UM_UPDATEDAMSGTRUE   WM_USER + 1
#define UM_UPDATEDAMSGFALSE   WM_USER + 2
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4996)
#include <string>
#include <cmath>
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
#include "afxcmn.h"
#include "afxwin.h"
using namespace std;

#define Video_flag 0
#define Audio_flag 1

///> Library Link On Windows System
#pragma comment( lib, "avformat.lib" )   
#pragma comment( lib, "avutil.lib" )
#pragma comment( lib, "avcodec.lib" )
#pragma comment( lib, "swscale.lib")
#pragma comment( lib, "avfilter.lib")




// CProjectEveDlg 대화 상자
class CProjectEveDlg : public CDialogEx
{
// 생성입니다.
public:
	CProjectEveDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	typedef struct _FilterContext {
		AVFilterGraph  *filter_graph;
		AVFilterContext* src_ctx;
		AVFilterContext* sink_ctx;
		AVFilterInOut*   inputs;
		AVFilterInOut*   outputs;
		int last_filter_idx = 0;
	} FilterContext;

	typedef struct _Codec_Set {
		AVCodec *codec = NULL;
		AVCodecContext *V_codec_ctx = NULL;
		AVCodecContext *A_codec_ctx = NULL;
	} Codec_Set;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROJECTEVE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLoad();
	
	char* find_Format(string input_format);

	int OpenImage(const char* imageFileName);
	Media* new_media;
	Media* test_media;
	AVFormatContext* init_input_format(const char* input_file_path);
	AVCodecContext* init_decoder(AVFormatContext *p_ifmt_ctx, int flag);
	AVCodecContext* init_encoder(AVFormatContext* p_ifmt_ctx, int flag);
	AVFormatContext* init_output_format(AVFormatContext* p_ifmt_ctx, AVFormatContext* pa_ifmt_ctx, const char* output_file_path, AVCodecContext* enc_codec_ctx, AVCodecContext* A_enc_codec_ctx);
	int init_video_filter();
	int init_video_filter_d();
	int set_video_filter(FilterContext* F_ctx, AVFilterInOut  *inputs, AVFilterInOut *outputs);
	int set_video_filter_d(FilterContext* F_ctx, AVFilterInOut  *inputs, AVFilterInOut *outputs);
	int insert_filter(FilterContext* F_ctx, char* filter_name, const char* command, int flag);
	int insert_filter_d(FilterContext* F_ctx, char* filter_name, const char* command, int flag);
	void release_video_filter();
	void release_video_filter_d();

	void setStreamInfo();
private:

	static UINT ThreadFirstData(LPVOID _mothod);
	static UINT ThreadSecondData(LPVOID _mothod);
	static UINT ThreadThirdData(LPVOID _mothod);

	AVInputFormat	*ifmt = NULL;
	AVOutputFormat  *ofmt = NULL;
	AVFormatContext *ifmt_ctx = NULL;
	AVFormatContext *a_ifmt_ctx = NULL;
	AVFormatContext *ofmt_ctx = NULL;
	
	AVFrame* filtered_frame;
	AVCodecContext* V_enc_codec_ctx = NULL;
	AVCodecContext* A_enc_codec_ctx = NULL;
	AVCodecContext* dec_V_ctx = NULL;
	AVCodecContext* dec_A_ctx = NULL;
	AVPacket read_pkt;
	AVPacket out_pkt;
	AVFrame* pVFrame;
	AVFrame* pAFrame;
	RGBImage* deg_Frame;
	int frame = 0;
	int deg_linewidth;
	CPoint from;
	CPoint to;
	int nVSI = -1;
	int nASI = -1;
	AVFrame *pictureFrame;
	AVFrame *m_picture_frame;

	int dst_width;
	int dst_height;
	int img_width;
	int img_height;

	int debugging_count;

	FilterContext vfilter_ctx, afilter_ctx;
	FilterContext vfilter_ctx_d, afilter_ctx_d;

	AVCodecID v_codec_id = AV_CODEC_ID_H264;
	AVCodecID a_codec_id = AV_CODEC_ID_MP3;

	char *szFilePath;
	char *outputfile = "output.mp4";
	char *audio_file = "audio_sample2.mp3";
	char *outputFormat;

	int m_watch_nbframe;
	int m_watch_pts;
	int m_watch_dts;
	int m_watch_size;

public:
	afx_msg LRESULT OnUpdateDataTrue(WPARAM w, LPARAM l);
	afx_msg LRESULT OnUpdateDataFalse(WPARAM w, LPARAM l);
	afx_msg void OnBnClickedButtonProcess();
	CProgressCtrl m_progress;
	CStatic m_status_text;
	CTreeCtrl m_tree;
	afx_msg void OnStnClickedStaticScaleHeight();
	int m_scale_width;
	int m_scale_height;
	afx_msg void OnBnClickedCheckFilter1();
	BOOL m_scale_checked;
	BOOL m_vflip_checked;
	BOOL m_hflip_checked;
	BOOL m_negative_checked;
	BOOL m_edge_checked;

	CListBox m_filter_list;
	CStatic m_picture_control;

	afx_msg void OnBnClickedCheckFilter2();
	afx_msg void OnBnClickedCheckFilter3();
	afx_msg void OnBnClickedCheckFilter4();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedCheckFilter5();
	afx_msg void OnBnClickedButtonDebug();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonNnext();
	afx_msg void OnBnClickedButton2();
	
	
};
