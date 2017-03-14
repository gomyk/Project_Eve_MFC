
// ProjectEveDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ProjectEve.h"
#include "ProjectEveDlg.h"
#include "afxdialogex.h"
#include "opencv2/opencv.hpp"   
#pragma comment(lib,"opencv_world320d.lib")

using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CProjectEveDlg 대화 상자



CProjectEveDlg::CProjectEveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROJECTEVE_DIALOG, pParent)
	, m_scale_width(1920)
	, m_scale_height(1080)
	, m_scale_checked(FALSE)
	, m_vflip_checked(FALSE)
	, m_hflip_checked(FALSE)
	, m_negative_checked(FALSE)
	, m_edge_checked(FALSE)
	, m_watch_nbframe(0)
	, m_watch_pts(0)
	, m_watch_dts(0)
	, m_watch_size(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProjectEveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_progress);
	DDX_Control(pDX, IDC_STATIC_STAT, m_status_text);
	DDX_Control(pDX, IDC_TREE_FRAME, m_tree);
	DDX_Text(pDX, IDC_EDIT_SCALE_WIDTH, m_scale_width);
	DDX_Text(pDX, IDC_EDIT_SCALE_HEIGHT, m_scale_height);
	DDX_Check(pDX, IDC_CHECK_FILTER_1, m_scale_checked);
	DDX_Check(pDX, IDC_CHECK_FILTER_2, m_vflip_checked);
	DDX_Check(pDX, IDC_CHECK_FILTER_3, m_hflip_checked);
	DDX_Check(pDX, IDC_CHECK_FILTER_4, m_negative_checked);
	DDX_Control(pDX, IDC_LIST_FILTER, m_filter_list);
	DDX_Check(pDX, IDC_CHECK_FILTER_5, m_edge_checked);
	DDX_Text(pDX, IDC_EDIT_WATCH, m_watch_nbframe);
	DDX_Text(pDX, IDC_EDIT4, m_watch_pts);
	DDX_Text(pDX, IDC_EDIT5, m_watch_dts);
	DDX_Text(pDX, IDC_EDIT6, m_watch_size);
	DDX_Control(pDX, IDC_PICTURE, m_picture_control);

}

BEGIN_MESSAGE_MAP(CProjectEveDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CProjectEveDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS, &CProjectEveDlg::OnBnClickedButtonProcess)
	ON_STN_CLICKED(IDC_STATIC_SCALE_HEIGHT, &CProjectEveDlg::OnStnClickedStaticScaleHeight)
	ON_BN_CLICKED(IDC_CHECK_FILTER_1, &CProjectEveDlg::OnBnClickedCheckFilter1)
	ON_BN_CLICKED(IDC_CHECK_FILTER_2, &CProjectEveDlg::OnBnClickedCheckFilter2)
	ON_BN_CLICKED(IDC_CHECK_FILTER_3, &CProjectEveDlg::OnBnClickedCheckFilter3)
	ON_BN_CLICKED(IDC_CHECK_FILTER_4, &CProjectEveDlg::OnBnClickedCheckFilter4)
	ON_BN_CLICKED(IDC_BUTTON_UP, &CProjectEveDlg::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, &CProjectEveDlg::OnBnClickedButtonDown)
	ON_BN_CLICKED(IDC_CHECK_FILTER_5, &CProjectEveDlg::OnBnClickedCheckFilter5)
	ON_BN_CLICKED(IDC_BUTTON_DEBUG, &CProjectEveDlg::OnBnClickedButtonDebug)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CProjectEveDlg::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON1, &CProjectEveDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_NNEXT, &CProjectEveDlg::OnBnClickedButtonNnext)
	ON_BN_CLICKED(IDC_BUTTON2, &CProjectEveDlg::OnBnClickedButton2)
	ON_MESSAGE(UM_UPDATEDAMSGFALSE, OnUpdateDataFalse)
	ON_MESSAGE(UM_UPDATEDAMSGTRUE, OnUpdateDataTrue)
END_MESSAGE_MAP()


// CProjectEveDlg 메시지 처리기

BOOL CProjectEveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	av_register_all();
	avfilter_register_all();
	m_progress.SetRange(0, 100);
	m_progress.SetPos(0);
	img_width = 80;
	debugging_count = 0;
	img_height = 80;
	
	//디버깅 필터 


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CProjectEveDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.
LRESULT CProjectEveDlg::OnUpdateDataTrue(WPARAM wParam, LPARAM lParam)
{
	return UpdateData(TRUE);
}
LRESULT CProjectEveDlg::OnUpdateDataFalse(WPARAM wParam, LPARAM lParam)
{
	return UpdateData(FALSE);
}
void CProjectEveDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CProjectEveDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CProjectEveDlg::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TCHAR szFilters[] = _T("Video Files (*.mp4)|*.mp4|All Files (*.*)|*.*||");

	CFileDialog fileDlg(TRUE, _T("mp4"), NULL,OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);
	// Display the file dialog. When user clicks OK, 
	CString fileName = NULL;
	CString pathName = NULL;

	wchar_t* wchar_str;
	int      char_str_len;  //char* 형 변수의 길이를 위한 변수

	//fileDlg.DoModal();
	if (fileDlg.DoModal() == IDOK)
	{
		pathName = fileDlg.GetPathName();
		fileName = fileDlg.GetFileTitle();

		wchar_str = pathName.GetBuffer(pathName.GetLength());
		char_str_len = WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, NULL, 0, NULL, NULL);
		szFilePath = new char[char_str_len];  //메모리 할당
											//wchar_t* to char* conversion
		WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, szFilePath, char_str_len, 0, 0);
	}
	new_media = new Media[3];//원본영상
	test_media = new Media[3];//합성영상
	ifmt_ctx = init_input_format(szFilePath);
	new_media[0].init_input_format(szFilePath);
	new_media[1].init_input_format("sample.mp4");
	new_media[2].init_input_format("sample2.mp4");
	test_media[0].init_input_format("massive.mp4");
	test_media[1].init_input_format("bbong.mp4");
	test_media[2].init_input_format("glare.mp4");
	// Data member in your dialog class 

		// Set its text (string loaded from string table) 
	CString text = _T("Load Complete");
	m_status_text.SetWindowText(text);
	//스트림정보 추가
	setStreamInfo();
	
	
	
	//CString message;
	//message.Format(_T("%d"), ifmt_ctx->streams[0]->codec->width);
	//MessageBox(_T("width : ")+message, fileName);
}
AVFormatContext* CProjectEveDlg::init_output_format(AVFormatContext* p_ifmt_ctx, AVFormatContext* pa_ifmt_ctx, const char* output_file_path, AVCodecContext* enc_codec_ctx, AVCodecContext* A_enc_codec_ctx) {
	AVOutputFormat   *p_ofmt = NULL;
	AVFormatContext  *p_ofmt_ctx = NULL;

	avformat_alloc_output_context2(&p_ofmt_ctx, NULL, outputFormat, outputfile);
	if (!p_ofmt_ctx)
	{
		fprintf(stderr, "Could not create output context\n");
	}

	p_ofmt = p_ofmt_ctx->oformat;

	int ret = 0;
	for (int i = 0; i < p_ifmt_ctx->nb_streams - 1; i++)
	{
		AVStream *in_stream = p_ifmt_ctx->streams[i];
		AVStream *out_stream = avformat_new_stream(p_ofmt_ctx, in_stream->codec->codec);

		if (p_ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			ret = avcodec_copy_context(out_stream->codec, enc_codec_ctx);
			out_stream->codec->codec_tag = 0;//문서
		}
		else if (p_ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			ret = avcodec_copy_context(out_stream->codec, A_enc_codec_ctx);
			out_stream->codec->codec_tag = 0;
		}

		if (ret < 0) {
			fprintf(stderr, "Failed to copy context from input to output stream codec context\n");
		}

		if (p_ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
		{
			out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
		}
	}
	AVStream *in_stream = pa_ifmt_ctx->streams[0];
	AVStream *out_stream = avformat_new_stream(p_ofmt_ctx, in_stream->codec->codec);

	if (pa_ifmt_ctx->streams[0]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
	{
		ret = avcodec_copy_context(out_stream->codec, enc_codec_ctx);
		out_stream->codec->codec_tag = 0;//문서
	}
	else if (pa_ifmt_ctx->streams[0]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
	{
		ret = avcodec_copy_context(out_stream->codec, A_enc_codec_ctx);
		out_stream->codec->codec_tag = 0;
	}

	if (ret < 0) {
		fprintf(stderr, "Failed to copy context from input to output stream codec context\n");
	}

	if (p_ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
	{
		out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
	}

	av_dump_format(p_ofmt_ctx, 0, output_file_path, 1);

	if (!(p_ofmt->flags & AVFMT_NOFILE))
	{
		ret = avio_open(&p_ofmt_ctx->pb, output_file_path, AVIO_FLAG_WRITE);
		if (ret < 0)
		{
			fprintf(stderr, "Could not open output file '%s'", output_file_path);
		}
	}

	ret = avformat_write_header(p_ofmt_ctx, NULL);
	if (ret < 0)
	{
		fprintf(stderr, "Error occurred when opening output file\n");
	}
	return p_ofmt_ctx;
}

AVCodecContext* CProjectEveDlg::init_encoder(AVFormatContext* p_ifmt_ctx, int flag) {

	AVCodec        *codec = NULL;
	AVCodecContext *c_ctx = NULL;

	if (flag == Video_flag) {
		if (nVSI != -1) {
			codec = avcodec_find_encoder(v_codec_id);
			if (!codec)
			{
				fprintf(stderr, "Codec not found\n");
				return 0;
			}

			c_ctx = avcodec_alloc_context3(codec);
			if (!c_ctx)
			{
				fprintf(stderr, "Could not allocate video codec context\n");
				return 0;
			}

			// Example
			if (v_codec_id == AV_CODEC_ID_H264)
				c_ctx->profile = FF_PROFILE_H264_BASELINE;
			c_ctx->width = dst_width;
			c_ctx->height = dst_height;
			c_ctx->pix_fmt = p_ifmt_ctx->streams[nVSI]->codec->pix_fmt;
			c_ctx->time_base = p_ifmt_ctx->streams[nVSI]->time_base;
			c_ctx->bit_rate = 5000 * 1000;
			c_ctx->time_base = AVRational{1,29};

			if (avcodec_open2(c_ctx, codec, NULL) < 0)
			{
				fprintf(stderr, "Could not open codec\n");
				return 0;
			}
		}
	}

	if (flag == Audio_flag) {
		if (nASI != -1) {
			codec = avcodec_find_encoder(a_codec_id);
			if (!codec)
			{
				fprintf(stderr, "Codec not found\n");
				return 0;
			}

			c_ctx = avcodec_alloc_context3(codec);
			if (!c_ctx)
			{
				fprintf(stderr, "Could not allocate video codec context\n");
				return 0;
			}

			c_ctx->bit_rate = 192 * 1000;
			c_ctx->time_base = { 1,44100 };
			c_ctx->sample_rate = 44100;
			c_ctx->channel_layout = p_ifmt_ctx->streams[0]->codec->channel_layout;
			c_ctx->channels = p_ifmt_ctx->streams[0]->codec->channels;
			c_ctx->sample_fmt = p_ifmt_ctx->streams[0]->codec->sample_fmt;

			// Example

			if (avcodec_open2(c_ctx, codec, NULL) < 0)
			{
				fprintf(stderr, "Could not open codec\n");
				return 0;
			}
		}
	}

	return c_ctx;
}

AVCodecContext* CProjectEveDlg::init_decoder(AVFormatContext *p_ifmt_ctx, int flag) {
	AVCodecContext *pVCtx = p_ifmt_ctx->streams[nVSI]->codec;
	AVCodecContext *pACtx = p_ifmt_ctx->streams[0]->codec;

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
		return 0;
	}

	///> Find Video Decoder
	if (flag == 0) {
		AVCodec   *pVideoCodec = avcodec_find_decoder(p_ifmt_ctx->streams[nVSI]->codec->codec_id);

		if (pVideoCodec == NULL)
		{
			av_log(NULL, AV_LOG_ERROR, "No Video Decoder was Found\n");
			return 0;
		}

		///> Initialize Codec Context as Decoder
		if (avcodec_open2(pVCtx, pVideoCodec, NULL) < 0)
		{
			av_log(NULL, AV_LOG_ERROR, "Fail to Initialize Decoder\n");
			return 0;
		}
	}

	///> Find Audio Decoder
	else {
		AVCodec *pAudioCodec = avcodec_find_decoder(p_ifmt_ctx->streams[0]->codec->codec_id);

		if (pAudioCodec == NULL)
		{
			av_log(NULL, AV_LOG_ERROR, "No Audio Decoder was Found\n");
			return 0;
		}

		///> Initialize Codec Context as Decoder
		if (avcodec_open2(pACtx, pAudioCodec, NULL) < 0)
		{
			av_log(NULL, AV_LOG_ERROR, "Fail to Initialize Decoder\n");
			return 0;
		}
	}

	if (flag) {
		return pACtx;
	}
	else {
		return pVCtx;
	}
}
int CProjectEveDlg::OpenImage(const char* imageFileName)
{
	AVFormatContext *picFormatCtx = NULL;
	if (avformat_open_input(&picFormatCtx, imageFileName, 0, 0) != 0)
	{
		printf("Can't open image file '%s'\n", imageFileName);
		return NULL;
	}

	av_dump_format(picFormatCtx, 0, imageFileName, false);

	AVCodecContext *pCodecCtx;

	pCodecCtx = picFormatCtx->streams[0]->codec;
	pCodecCtx->width = picFormatCtx->streams[0]->codec->width;
	pCodecCtx->height = picFormatCtx->streams[0]->codec->height;
	pCodecCtx->pix_fmt = picFormatCtx->streams[0]->codec->pix_fmt;

	// Find the decoder for the video stream
	AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (!pCodec)
	{
		printf("Codec not found\n");
		return NULL;
	}

	// Open codec
	if (avcodec_open2(pCodecCtx, pCodec, NULL)<0)
	{
		printf("Could not open codec\n");
		return NULL;
	}

	// 

	m_picture_frame = av_frame_alloc();
	av_free( pictureFrame);
	pictureFrame = av_frame_alloc();

	if (!pictureFrame)
	{
		printf("Can't allocate memory for AVFrame\n");
		return NULL;
	}

	int frameFinished;
	int numBytes;

	// Determine required buffer size and allocate buffer
	numBytes = avpicture_get_size(pCodecCtx->pix_fmt, img_width, img_height);
	uint8_t *buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));

	avpicture_fill((AVPicture *)pictureFrame, buffer, ifmt_ctx->streams[nVSI]->codec->pix_fmt, img_width, img_height);
	pictureFrame->quality = 10;
	// Read frame
	delete buffer;
	AVPacket packet;

	int framesNumber = 0;
	while (av_read_frame(picFormatCtx, &packet) >= 0)
	{
		if (packet.stream_index != 0)
			continue;

		int ret = avcodec_decode_video2(pCodecCtx, pictureFrame, &frameFinished, &packet);

		if (ret > 0)
		{
			FilterContext* imagefilter_ctx = &vfilter_ctx;
			if (av_buffersrc_add_frame(imagefilter_ctx->src_ctx, pictureFrame) < 0)
			{
				printf("Error occurred when putting frame into filter context\n");
			}
			while (1)
			{
				// Get frame from filter, if it returns < 0 then filter is currently empty.
				if (av_buffersink_get_frame(imagefilter_ctx->sink_ctx, m_picture_frame) < 0)
				{
					break;
				}
			} // while
			printf("Frame is decoded, size %d\n", ret);
			
			return 1;
		}
		else
			printf("Error [%d] while decoding frame: %s\n", ret, strerror(AVERROR(ret)));
	}
}
char* CProjectEveDlg::find_Format(string input_format) {
	if (input_format.find("mp4") != -1)
	{
		return "mp4";
	}
	else if (input_format.find("avi") != -1)
	{
		return "avi";
	}
	else if (input_format.find("flv") != -1)
	{
		return "flv";
	}
	else
	{
		return "ts";
	}
}
AVFormatContext* CProjectEveDlg::init_input_format(const char* input_file_path) {

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
		return 0;
	}

	return p_ifmt_ctx;
}
int CProjectEveDlg::insert_filter(FilterContext* F_ctx, char* filter_name, const char* command, int flag)
{
	if (flag == 0) {
		AVFilterContext      *filter_ctx;
		char args[512];
		int last_filter_idx = F_ctx->last_filter_idx;
		AVFilterContext* last_filter = F_ctx->filter_graph->filters[last_filter_idx];
		if (command == NULL) {
			command = "";
		}

		AVFilter* What_Filter = avfilter_get_by_name(filter_name);

		snprintf(args, sizeof(args), "%s", command);
		if (avfilter_graph_create_filter(&filter_ctx, avfilter_get_by_name(filter_name), filter_name, args, NULL, F_ctx->filter_graph) < 0)
		{
			printf("Failed to create video scale filter\n");
			return -4;
		}

		if (F_ctx->last_filter_idx == 2) {
			if (avfilter_link(F_ctx->outputs->filter_ctx, 0, filter_ctx, 0) < 0)
			{
				printf("Failed to link video format filter\n");
				return -4;
			}
		}
		else {

			if (avfilter_link(last_filter, 0, filter_ctx, 0) < 0)
			{
				printf("Failed to link video format filter\n");
				return -4;
			}
		}

		F_ctx->last_filter_idx = F_ctx->filter_graph->nb_filters - 1;
	}
	return 0;
}

int CProjectEveDlg::init_video_filter()
{
	AVStream       *stream =ifmt_ctx->streams[0];
	AVCodecContext   *codec_ctx = stream->codec;

	AVFilterContext     *rotate_filter;
	AVFilterInOut     *inputs, *outputs;

	char args[512];

	vfilter_ctx.filter_graph = NULL;
	vfilter_ctx.src_ctx = NULL;
	vfilter_ctx.sink_ctx = NULL;

	// Allocate memory for filter graph
	vfilter_ctx.filter_graph = avfilter_graph_alloc();
	if (vfilter_ctx.filter_graph == NULL)
	{
		return -1;
	}

	// Link input and output with filter graph.
	if (avfilter_graph_parse2(vfilter_ctx.filter_graph, "null", &inputs, &outputs) < 0)
	{
		printf("Failed to parse video filtergraph\n");
		return -2;
	}

	// Create input filter
	// Create Buffer Source -> input filter
	snprintf(args, sizeof(args), "time_base=%d/%d:video_size=%dx%d:pix_fmt=%d:pixel_aspect=%d/%d"
		, stream->time_base.num, stream->time_base.den
		, codec_ctx->width, codec_ctx->height
		, codec_ctx->pix_fmt
		, codec_ctx->sample_aspect_ratio.num, codec_ctx->sample_aspect_ratio.den);

	// Create Buffer Source
	if (avfilter_graph_create_filter(
		&vfilter_ctx.src_ctx
		, avfilter_get_by_name("buffer")
		, "in", args, NULL, vfilter_ctx.filter_graph) < 0)
	{
		printf("Failed to create video buffer source\n");
		return -3;
	}

	// Link Buffer Source with input filter
	if (avfilter_link(vfilter_ctx.src_ctx, 0, inputs->filter_ctx, 0) < 0)
	{
		printf("Failed to link video buffer source\n");
		return -4;
	}

	// Create Output filter
	// Create Buffer Sink
	if (avfilter_graph_create_filter(
		&vfilter_ctx.sink_ctx
		, avfilter_get_by_name("buffersink")
		, "out", NULL, NULL, vfilter_ctx.filter_graph) < 0)
	{
		printf("Failed to create video buffer sink\n");
		return -3;
	}

	// 필터 핵심 구역. 여기서 사용 할 필터의 종류와, 명령어 값을 전송함.
	//snprintf(args, sizeof(args), "%d%s", 70, " * PI / 180");
	//if (avfilter_graph_create_filter(&rotate_filter, avfilter_get_by_name("rotate"), "Rotate Filter", args, NULL, vfilter_ctx.filter_graph) < 0)
	//{
	//   printf("Failed to create video scale filter\n");
	//   return -4;
	//}

	//// link rescaler filter with aformat filter
	//if (avfilter_link(outputs->filter_ctx, 0, rotate_filter, 0) < 0)
	//{
	//   printf("Failed to link video format filter\n");
	//   return -4;
	//}

	vfilter_ctx.last_filter_idx = vfilter_ctx.filter_graph->nb_filters - 1;
	vfilter_ctx.inputs = inputs;
	vfilter_ctx.outputs = outputs;
}
void  CProjectEveDlg::release_video_filter() {
	avfilter_graph_free(&vfilter_ctx.filter_graph);
	avfilter_graph_free(&afilter_ctx.filter_graph);
}
int CProjectEveDlg::set_video_filter(FilterContext* F_ctx, AVFilterInOut  *inputs, AVFilterInOut *outputs) {

	// stream 관련 수정 필요.
	AVFilterContext* last_filter;
	last_filter = F_ctx->filter_graph->filters[F_ctx->last_filter_idx];

	AVStream         *stream = ifmt_ctx->streams[nVSI];
	AVCodecContext      *codec_ctx = stream->codec;

	// aformat is linked with Buffer Sink filter.
	if (avfilter_link(last_filter, 0, F_ctx->sink_ctx, 0) < 0)
	{
		printf("Failed to link video format filter\n");
		return -4;
	}

	// Configure all prepared filters.
	if (avfilter_graph_config(F_ctx->filter_graph, NULL) < 0)
	{
		printf("Failed to configure video filter context\n");
		return -5;
	}

	av_buffersink_set_frame_size(F_ctx->sink_ctx, codec_ctx->frame_size);

	int ret = avfilter_graph_config(F_ctx->filter_graph, NULL);

	avfilter_inout_free(&F_ctx->inputs);
	avfilter_inout_free(&F_ctx->outputs);
}

int CProjectEveDlg::init_video_filter_d()
{
	AVStream       *stream = ifmt_ctx->streams[0];
	AVCodecContext   *codec_ctx = stream->codec;

	AVFilterContext     *rotate_filter;
	AVFilterInOut     *inputs, *outputs;

	char args[512];

	vfilter_ctx_d.filter_graph = NULL;
	vfilter_ctx_d.src_ctx = NULL;
	vfilter_ctx_d.sink_ctx = NULL;

	// Allocate memory for filter graph
	vfilter_ctx_d.filter_graph = avfilter_graph_alloc();
	if (vfilter_ctx_d.filter_graph == NULL)
	{
		return -1;
	}

	// Link input and output with filter graph.
	if (avfilter_graph_parse2(vfilter_ctx_d.filter_graph, "null", &inputs, &outputs) < 0)
	{
		printf("Failed to parse video filtergraph\n");
		return -2;
	}

	// Create input filter
	// Create Buffer Source -> input filter
	snprintf(args, sizeof(args), "time_base=%d/%d:video_size=%dx%d:pix_fmt=%d:pixel_aspect=%d/%d"
		, stream->time_base.num, stream->time_base.den
		, codec_ctx->width, codec_ctx->height
		, codec_ctx->pix_fmt
		, codec_ctx->sample_aspect_ratio.num, codec_ctx->sample_aspect_ratio.den);

	// Create Buffer Source
	if (avfilter_graph_create_filter(
		&vfilter_ctx_d.src_ctx
		, avfilter_get_by_name("buffer")
		, "in", args, NULL, vfilter_ctx_d.filter_graph) < 0)
	{
		printf("Failed to create video buffer source\n");
		return -3;
	}

	// Link Buffer Source with input filter
	if (avfilter_link(vfilter_ctx_d.src_ctx, 0, inputs->filter_ctx, 0) < 0)
	{
		printf("Failed to link video buffer source\n");
		return -4;
	}

	// Create Output filter
	// Create Buffer Sink
	if (avfilter_graph_create_filter(
		&vfilter_ctx_d.sink_ctx
		, avfilter_get_by_name("buffersink")
		, "out", NULL, NULL, vfilter_ctx_d.filter_graph) < 0)
	{
		printf("Failed to create video buffer sink\n");
		return -3;
	}

	// 필터 핵심 구역. 여기서 사용 할 필터의 종류와, 명령어 값을 전송함.
	//snprintf(args, sizeof(args), "%d%s", 70, " * PI / 180");
	//if (avfilter_graph_create_filter(&rotate_filter, avfilter_get_by_name("rotate"), "Rotate Filter", args, NULL, vfilter_ctx.filter_graph) < 0)
	//{
	//   printf("Failed to create video scale filter\n");
	//   return -4;
	//}

	//// link rescaler filter with aformat filter
	//if (avfilter_link(outputs->filter_ctx, 0, rotate_filter, 0) < 0)
	//{
	//   printf("Failed to link video format filter\n");
	//   return -4;
	//}

	vfilter_ctx_d.last_filter_idx = vfilter_ctx_d.filter_graph->nb_filters - 1;
	vfilter_ctx_d.inputs = inputs;
	vfilter_ctx_d.outputs = outputs;
}
void  CProjectEveDlg::release_video_filter_d() {
	avfilter_graph_free(&vfilter_ctx_d.filter_graph);
	avfilter_graph_free(&vfilter_ctx_d.filter_graph);
}
int CProjectEveDlg::set_video_filter_d(FilterContext* F_ctx, AVFilterInOut  *inputs, AVFilterInOut *outputs) {

	// stream 관련 수정 필요.
	AVFilterContext* last_filter;
	last_filter = F_ctx->filter_graph->filters[F_ctx->last_filter_idx];

	AVStream         *stream = ifmt_ctx->streams[nVSI];
	AVCodecContext      *codec_ctx = stream->codec;

	// aformat is linked with Buffer Sink filter.
	if (avfilter_link(last_filter, 0, F_ctx->sink_ctx, 0) < 0)
	{
		printf("Failed to link video format filter\n");
		return -4;
	}

	// Configure all prepared filters.
	if (avfilter_graph_config(F_ctx->filter_graph, NULL) < 0)
	{
		printf("Failed to configure video filter context\n");
		return -5;
	}

	av_buffersink_set_frame_size(F_ctx->sink_ctx, codec_ctx->frame_size);

	int ret = avfilter_graph_config(F_ctx->filter_graph, NULL);

	avfilter_inout_free(&F_ctx->inputs);
	avfilter_inout_free(&F_ctx->outputs);
}
int CProjectEveDlg::insert_filter_d(FilterContext* F_ctx, char* filter_name, const char* command, int flag)
{
	if (flag == 0) {
		AVFilterContext      *filter_ctx;
		char args[512];
		int last_filter_idx = F_ctx->last_filter_idx;
		AVFilterContext* last_filter = F_ctx->filter_graph->filters[last_filter_idx];
		if (command == NULL) {
			command = "";
		}

		AVFilter* What_Filter = avfilter_get_by_name(filter_name);

		snprintf(args, sizeof(args), "%s", command);
		if (avfilter_graph_create_filter(&filter_ctx, avfilter_get_by_name(filter_name), filter_name, args, NULL, F_ctx->filter_graph) < 0)
		{
			printf("Failed to create video scale filter\n");
			return -4;
		}

		if (F_ctx->last_filter_idx == 2) {
			if (avfilter_link(F_ctx->outputs->filter_ctx, 0, filter_ctx, 0) < 0)
			{
				printf("Failed to link video format filter\n");
				return -4;
			}
		}
		else {

			if (avfilter_link(last_filter, 0, filter_ctx, 0) < 0)
			{
				printf("Failed to link video format filter\n");
				return -4;
			}
		}

		F_ctx->last_filter_idx = F_ctx->filter_graph->nb_filters - 1;
	}
	return 0;
}

void CProjectEveDlg::OnBnClickedButtonProcess()
{
}
void CProjectEveDlg::setStreamInfo() {
	CString text;
	HTREEITEM  hRoot;
	HTREEITEM  hChild;
	for (int i = 0; i < ifmt_ctx->nb_streams; i++) {
		if (ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
			text = _T("Video Stream");
		else
			text = _T("Audio Stream");
		hRoot = m_tree.InsertItem(text, 0/* nImage */, 1/* nSelectedImage */, TVI_ROOT, TVI_LAST);

		if (ifmt_ctx->streams[i]->codec->codec_id == AV_CODEC_ID_H264) {
			text = _T("codec : H264");
		}
		hChild = m_tree.InsertItem(text, 1/* nImage */, 1/* nSelectedImage */, hRoot, TVI_LAST);
	}
}

void CProjectEveDlg::OnStnClickedStaticScaleHeight()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}


void CProjectEveDlg::OnBnClickedCheckFilter1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;
	CString cur;
	UpdateData(true);
	if (m_scale_checked) {
		m_filter_list.AddString(_T("Scale filter"));
	}
	else {
		temp = "Scale filter";
		for (int i = 0; i < m_filter_list.GetCount(); i++) {
			m_filter_list.GetText(i, cur);
			if (cur == temp) {
				m_filter_list.DeleteString(i);
			}
		}
	}
}


void CProjectEveDlg::OnBnClickedCheckFilter2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;
	CString cur;
	UpdateData(true);
	if (m_vflip_checked) {
		m_filter_list.AddString(_T("Vflip filter"));
	}
	else {
		temp = "Vflip filter";
		for (int i = 0; i < m_filter_list.GetCount(); i++) {
			m_filter_list.GetText(i, cur);
			if (cur == temp) {
				m_filter_list.DeleteString(i);
			}
		}
	}
}


void CProjectEveDlg::OnBnClickedCheckFilter3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;
	CString cur;
	UpdateData(true);
	if (m_hflip_checked) {
		m_filter_list.AddString(_T("Hflip filter"));
	}
	else {
		temp = "Hflip filter";
		for (int i = 0; i < m_filter_list.GetCount(); i++) {
			m_filter_list.GetText(i, cur);
			if (cur == temp) {
				m_filter_list.DeleteString(i);
			}
		}
	}
}


void CProjectEveDlg::OnBnClickedCheckFilter4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;
	CString cur;
	UpdateData(true);
	if (m_negative_checked) {
		m_filter_list.AddString(_T("Negative filter"));
	}
	else {
		temp = "Negative filter";
		for (int i = 0; i < m_filter_list.GetCount(); i++) {
			m_filter_list.GetText(i, cur);
			if (cur == temp) {
				m_filter_list.DeleteString(i);
			}
		}
	}
}


void CProjectEveDlg::OnBnClickedButtonUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString now;
	CString next;
	int index = m_filter_list.GetCurSel();
	if (index > 0) {
		m_filter_list.GetText(index, now);
		m_filter_list.GetText(index - 1, next);
		m_filter_list.DeleteString(index - 1);
		m_filter_list.InsertString(index - 1, now);
		m_filter_list.DeleteString(index);
		m_filter_list.InsertString(index, next);
		m_filter_list.SetCurSel(index-1);
	}
	else {

	}
}


void CProjectEveDlg::OnBnClickedButtonDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString now;
	CString next;
	int index = m_filter_list.GetCurSel();
	if (index+1 < m_filter_list.GetCount()) {
		m_filter_list.GetText(index, now);
		m_filter_list.GetText(index + 1, next);
		m_filter_list.DeleteString(index + 1);
		m_filter_list.InsertString(index + 1, now);
		m_filter_list.DeleteString(index);
		m_filter_list.InsertString(index, next);
		m_filter_list.SetCurSel(index + 1);
	}
	else {

	}
}


void CProjectEveDlg::OnBnClickedCheckFilter5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;
	CString cur;
	UpdateData(true);
	if (m_edge_checked) {
		m_filter_list.AddString(_T("Edge filter"));
	}
	else {
		temp = "Edge filter";
		for (int i = 0; i < m_filter_list.GetCount(); i++) {
			m_filter_list.GetText(i, cur);
			if (cur == temp) {
				m_filter_list.DeleteString(i);
			}
		}
	}
}


void CProjectEveDlg::OnBnClickedButtonDebug()
{
	Mat image = imread("art.jpg", 1);

	imshow("window", image);
}

void CProjectEveDlg::OnBnClickedButtonNext()
{
	
}


void CProjectEveDlg::OnBnClickedButton1()
{


}


void CProjectEveDlg::OnBnClickedButtonNnext()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int bGotPicture = 1;
	int bGotSound = 1;
	int count = 0;
	bool next = true;
	while (av_read_frame(ifmt_ctx, &read_pkt) >= 0) {
		///> Decoding
		count++;
		frame++;
		m_watch_nbframe = frame;
		UpdateData(FALSE);
		//av_read_frame(a_ifmt_ctx, &apkt);
		if (read_pkt.stream_index == nVSI && count == 10) {

			next = false;
			break;

			if (avcodec_decode_video2(dec_V_ctx, pVFrame, &bGotPicture, &read_pkt) >= 0) {
				if (bGotPicture) {
					FilterContext* filter_ctx = &vfilter_ctx;
					out_pkt.data = NULL;    // packet data will be allocated by the encoder
					out_pkt.size = 0;
					if (av_buffersrc_add_frame(filter_ctx->src_ctx, pVFrame) < 0)
					{
						printf("Error occurred when putting frame into filter context\n");
					}
					while (1)
					{
						// Get frame from filter, if it returns < 0 then filter is currently empty.
						if (av_buffersink_get_frame(filter_ctx->sink_ctx, filtered_frame) < 0)
						{
							break;
						}
					} // while
					  //효과 필터 적용 시점
					int start = 5 * ofmt_ctx->streams[0]->codec->time_base.den;
					int end = 10 * ofmt_ctx->streams[0]->codec->time_base.den;

					//추가
					RGBImage* frameimage;

					frameimage = new RGBImage(dst_width, dst_height);
					frameimage->copyFrame(filtered_frame);
					u_char R, G, B;
			
					CDC* p = m_picture_control.GetWindowDC();


					for (int i = 0; i < 300; i++) {
						for (int j = 0; j < 500; j++) {
							CPoint temp(j, i);
							frameimage->getPixelColor(R, G, B, i*filtered_frame->linesize[0] + j);
							p->SetPixelV(temp, RGB(R, G, B));
						}
					}


					m_picture_control.ReleaseDC(p);

					int H = dst_height;
					int W = filtered_frame->linesize[0];
					for (int i = 0; i < H; i++) {
						for (int j = 0; j < dst_width; j++) {

							if (m_picture_frame->data[0][i*W + j] <200) {
								filtered_frame->data[0][i*W + j] = m_picture_frame->data[0][i*W + j];
								filtered_frame->data[1][((i / 2)*(W / 2) + j / 2)] = (m_picture_frame->data[1][((i / 2)*(W / 2) + j / 2)] + filtered_frame->data[1][((i / 2)*(W / 2) + j / 2)]) / 2;
								filtered_frame->data[2][((i / 2)*(W / 2) + j / 2)] = (m_picture_frame->data[2][((i / 2)*(W / 2) + j / 2)] + filtered_frame->data[2][((i / 2)*(W / 2) + j / 2)]) / 2;
							}

						}

					}

					if (frame < start) {
						//filtered_frame->pts = av_frame_get_best_effort_timestamp(pVFrame);
						avcodec_encode_video2(V_enc_codec_ctx, &out_pkt, filtered_frame, &bGotPicture);

					}
					else if (frame > end) {
						//filtered_frame->pts = av_frame_get_best_effort_timestamp(pVFrame);
						avcodec_encode_video2(V_enc_codec_ctx, &out_pkt, filtered_frame, &bGotPicture);
					}
					else {
						m_picture_frame->pts = pVFrame->pts;
						avcodec_encode_video2(V_enc_codec_ctx, &out_pkt, m_picture_frame, &bGotPicture);
					}
				
					av_frame_unref(filtered_frame);
					if (bGotPicture) {
						av_interleaved_write_frame(ofmt_ctx, &out_pkt);
						m_watch_pts = out_pkt.pts;
						m_watch_dts = out_pkt.dts;
						m_watch_size = out_pkt.size;
						UpdateData(FALSE);
						m_progress.SetPos(frame);
						av_free_packet(&out_pkt);
					}
					next = false;
					break;
				}
			}
		
		}
		av_free_packet(&read_pkt);
	}
	if(av_read_frame(ifmt_ctx, &read_pkt) < 0 && next) {
		CString text;
		for (bGotPicture = 1; bGotPicture; ) {
			fflush(stdout);
			if (avcodec_encode_video2(V_enc_codec_ctx, &read_pkt, NULL, &bGotPicture) < 0) {
				fprintf(stderr, "Error encoding frame\n");
				exit(1);
			}
			if (bGotPicture) {
				av_interleaved_write_frame(ofmt_ctx, &read_pkt);
				av_free_packet(&read_pkt);
			}
		}
		text = _T("encoding Audio..");
		m_status_text.SetWindowText(text);
		while (av_read_frame(a_ifmt_ctx, &read_pkt) >= 0) {
			if (read_pkt.stream_index == 0) {
				if (read_pkt.data != NULL) {
					av_interleaved_write_frame(ofmt_ctx, &out_pkt);
					av_free_packet(&out_pkt);
					if (avcodec_decode_audio4(dec_A_ctx, pAFrame, &bGotSound, &read_pkt) >= 0) {
						if (bGotSound) {
							avcodec_encode_audio2(A_enc_codec_ctx, &out_pkt, pAFrame, &bGotSound);
							if (bGotSound) {
								out_pkt.stream_index = 1;
								out_pkt.pts = out_pkt.pts / 330;
								out_pkt.dts = out_pkt.dts / 330;
								av_interleaved_write_frame(ofmt_ctx, &out_pkt);
								frame++;
								if (frame > 31 * 396 / 10) {
									break;
								}
								av_free_packet(&out_pkt);
							}
						}
					}
				}
			}
			av_free_packet(&read_pkt);
		}
		av_write_trailer(ofmt_ctx);
		avcodec_close(V_enc_codec_ctx);
		av_free(V_enc_codec_ctx);
		av_free(pVFrame);
		av_free(pAFrame);
		///> Close an opened input AVFormatContext. 
		avformat_close_input(&ifmt_ctx);
		avformat_close_input(&ofmt_ctx);
		text = _T("Done!!");
		m_status_text.SetWindowText(text);
		CString strPath = _T("C:\\Users\\itaDev1\\Documents\\Visual Studio 2015\\Projects\\ProjectEve\\ProjectEve\\output.mp4");
		ShellExecute(NULL, _T("open"), _T("explorer"), _T("/select,") + strPath, NULL, SW_SHOW);
	}
}
UINT CProjectEveDlg::ThreadFirstData(LPVOID _mothod) {//Thread부분
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	CProjectEveDlg *fir = (CProjectEveDlg*)_mothod;
	CProjectEveDlg* pDlg = (CProjectEveDlg*)AfxGetApp()->m_pMainWnd;
	
	srand(time(NULL));
	char args[512];
	pDlg->PostMessage(UM_UPDATEDAMSGTRUE, 0, 0);

	CString text = _T("Start Encoding");
	pDlg->dst_width = pDlg->m_scale_width;
	pDlg->dst_height = pDlg->m_scale_height;

	pDlg->img_width = pDlg->m_scale_width;
	pDlg->img_height = pDlg->m_scale_height;
	pDlg->m_status_text.SetWindowText(text);
	pDlg->init_video_filter();

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//pDlg->new_media[0].init_input_format(szFilePath);
	pDlg->a_ifmt_ctx = pDlg->init_input_format(pDlg->audio_file);
	pDlg->outputFormat = pDlg->find_Format(pDlg->new_media[0].ifmt_ctx->iformat->name);

	pDlg->init_video_filter();
	CString temp;
	for (int i = 0; i < pDlg->m_filter_list.GetCount(); i++) {
		pDlg->m_filter_list.GetText(i, temp);
		if (temp == "Scale filter") {
			snprintf(args, sizeof(args), "%d:%d", pDlg->m_scale_width, pDlg->m_scale_height);
			pDlg->insert_filter(&pDlg->vfilter_ctx, "scale", args, 0);
		}
		else if (temp == "Vflip filter")
			pDlg->insert_filter(&pDlg->vfilter_ctx, "vflip", NULL, 0);
		else if (temp == "Hflip filter")
			pDlg->insert_filter(&pDlg->vfilter_ctx, "hflip", NULL, 0);
		else if (temp == "Negative filter")
			pDlg->insert_filter(&pDlg->vfilter_ctx, "negate", NULL, 0);
		else  if (temp == "Edge filter")
			pDlg->insert_filter(&pDlg->vfilter_ctx, "convolution", "0 1 0 1 -4 1 0 1 0:0 1 0 1 -4 1 0 1 0:0 1 0 1 -4 1 0 1 0:0 1 0 1 -4 1 0 1 0:5:5:5:1:0:128:128:128", 0);
	}
	pDlg->set_video_filter(&pDlg->vfilter_ctx, pDlg->vfilter_ctx.inputs, pDlg->vfilter_ctx.outputs);   // 삽입된 필터 정리

	pDlg->init_video_filter_d();
	snprintf(args, sizeof(args), "%d:%d", 500, 300);
	pDlg->insert_filter_d(&pDlg->vfilter_ctx_d, "scale", args, 0);
	pDlg->set_video_filter_d(&pDlg->vfilter_ctx_d, pDlg->vfilter_ctx_d.inputs, pDlg->vfilter_ctx_d.outputs);

	AVCodecContext* V_enc_codec_ctx = NULL;
	AVCodecContext* A_enc_codec_ctx = NULL;
	//추가
	AVCodecContext* temp_ctx = NULL;

	temp_ctx = pDlg->init_encoder(pDlg->new_media[0].ifmt_ctx, Video_flag);;
	temp_ctx->width = pDlg->m_scale_width;
	temp_ctx->height = pDlg->m_scale_height;
	temp_ctx->pix_fmt = pDlg->new_media[0].ifmt_ctx->streams[pDlg->nVSI]->codec->pix_fmt;
	temp_ctx->time_base = pDlg->new_media[0].ifmt_ctx->streams[pDlg->nVSI]->time_base;
	temp_ctx->bit_rate = 50000 * 1000;
	for (int i = 0; i < 3; i++) {

		pDlg->test_media[i].init_encoder(Video_flag, AV_CODEC_ID_H264, temp_ctx);
	}
	

	V_enc_codec_ctx = pDlg->init_encoder(pDlg->new_media[0].ifmt_ctx, Video_flag);
	A_enc_codec_ctx = pDlg->init_encoder(pDlg->a_ifmt_ctx, Audio_flag);

	AVCodecContext* dec_V_ctx = NULL;
	AVCodecContext* dec_A_ctx = NULL;
	//추가
	for (int i = 0; i < 3; i++) {
		pDlg->new_media[i].init_decoder(Video_flag);
		pDlg->test_media[i].init_decoder(Video_flag);
	}

	pDlg->init_decoder(pDlg->new_media[0].ifmt_ctx, Video_flag);
	dec_A_ctx = pDlg->init_decoder(pDlg->a_ifmt_ctx, Audio_flag);
	pDlg->ofmt_ctx = pDlg->init_output_format(pDlg->test_media[0].ifmt_ctx, pDlg->a_ifmt_ctx, pDlg->outputfile, pDlg->test_media[0].enc_codec_ctx, A_enc_codec_ctx);

	AVPacket read_pkt, read_pkt2;
	AVPacket out_pkt, out_pkt2,out_pkt3;
	AVFrame* pVFrame;
	AVFrame* pAFrame;

	int bGotPicture = 0;   // flag for video decoding
	int bGotSound = 0;      // flag for audio decoding

	pVFrame = av_frame_alloc();
	pAFrame = av_frame_alloc();

	av_init_packet(&out_pkt);
	av_init_packet(&out_pkt2);
	av_init_packet(&out_pkt3);
	// 비디오 필터 설정
	//init_video_filter();
	int stream_index;

	AVFrame* filtered_frame = av_frame_alloc();
	AVFrame* filtered_frame2 = av_frame_alloc();
	AVFrame* filtered_frame3 = av_frame_alloc();
	
	//8level grayscale 변화 
	/*
	for (int i = 0; i < dst_width * (dst_height); i++) {
	m_picture_frame->data[0][i] = (m_picture_frame->data[0][i] / 32) * 32;//0~255 Y
	if (i < dst_width * ((dst_height+32) / 4)) {
	m_picture_frame->data[1][i] = 127;//0~255 U
	m_picture_frame->data[2][i] = 127;//0~255 V
	}

	}*/
	//Sepia 필터
	/*
	for (int i = 0; i < dst_width * (dst_height); i++) {
	if (i < dst_width * ((dst_height+32) / 4)) {
	m_picture_frame->data[1][i] = 113;//0~255 U
	m_picture_frame->data[2][i] = 143;//0~255 V
	}

	}*/
	//반전
	/*
	for (int i = 0; i < dst_width * (dst_height); i++) {
	m_picture_frame->data[0][i] = 255- m_picture_frame->data[0][i];//0~255 Y
	if (i < dst_width * ((dst_height+32) / 4)) {
	m_picture_frame->data[1][i] = 255- m_picture_frame->data[1][i];//0~255 U
	m_picture_frame->data[2][i] = 255- m_picture_frame->data[2][i];//0~255 V
	}

	}*/

	text = _T("encoding Video..");
	pDlg->m_status_text.SetWindowText(text);
	int i = 0;
	int frames = pDlg->test_media[0].ifmt_ctx->streams[0]->nb_frames;
	pDlg->m_progress.SetRange(0, frames);
	pDlg->m_progress.SetPos(i);
	bool s_f = false;
	bool d_f = false;
	bool sw = false;
	int timer = 0;
	double calc_time_base = 30;//pDlg->test_media[0].calc_time_base();
	long long now_pts = 0;
	int N = 0;
	int select = 0;
	int select_origin = 0;
	int back = 0;
	while (N < calc_time_base *30) {//합성할 영상  //알고리즘 생각 
		RGBImage* frame;
		if (N < 5*calc_time_base) {
			select = 0;
			
		}
		else if (N < 10 * calc_time_base) {
			sw = true;
		}
		else if (N < 15 * calc_time_base) {
			back = 0;
			sw = false;
			select = 1;
		}
		else if (N < 20 * calc_time_base) {
			sw = true;
			select_origin = 2;
		}
		else {
			sw = false;
			select = 2;
		}
		
		if (!s_f) {
			if (av_read_frame(pDlg->new_media[select].ifmt_ctx, &read_pkt2) >= 0) {
				///> Decoding
				//av_read_frame(a_ifmt_ctx, &apkt);
				if (read_pkt2.stream_index == pDlg->nVSI) {

					pDlg->PostMessage(UM_UPDATEDAMSGFALSE, 0, 0);
					if (avcodec_decode_video2(pDlg->new_media[select].dec_ctx, pVFrame, &bGotPicture, &read_pkt2) >= 0) {
						if (bGotPicture) {
							FilterContext* filter_ctx = &pDlg->vfilter_ctx;
							out_pkt2.data = NULL;    // packet data will be allocated by the encoder
							out_pkt2.size = 0;

							if (av_buffersrc_add_frame(filter_ctx->src_ctx, pVFrame) < 0)
							{
								printf("Error occurred when putting frame into filter context\n");
								break;
							}
							while (1)
							{
								// Get frame from filter, if it returns < 0 then filter is currently empty.
								if (av_buffersink_get_frame(filter_ctx->sink_ctx, filtered_frame2) < 0)
								{
									break;
								}
							} // while
							  //효과 필터 적용 시점
							frame = new RGBImage(pDlg->dst_width, pDlg->dst_height);
							frame->copyFrame(filtered_frame2);
							s_f = true;
							/*
							avcodec_encode_video2(V_enc_codec_ctx, &out_pkt2, filtered_frame2, &bGotPicture);

							if (bGotPicture) {

							//av_interleaved_write_frame(ofmt_ctx, &out_pkt2);
							}*/
							av_frame_unref(filtered_frame2);
						}
					}
				}
				av_free_packet(&read_pkt2);
			}
			//
			if (sw && frame->isGood()) {
				bool good = false;
				while (!good) {
					if (av_read_frame(pDlg->new_media[select + 1].ifmt_ctx, &read_pkt) >= 0) {
						///> Decoding
						//av_read_frame(a_ifmt_ctx, &apkt);
						if (read_pkt.stream_index == pDlg->nVSI) {
							if (avcodec_decode_video2(pDlg->new_media[select + 1].dec_ctx, pVFrame, &bGotPicture, &read_pkt) >= 0) {
								if (bGotPicture) {
									FilterContext* filter_ctx = &pDlg->vfilter_ctx;
									out_pkt3.data = NULL;    // packet data will be allocated by the encoder
									out_pkt3.size = 0;
									if (av_buffersrc_add_frame(filter_ctx->src_ctx, pVFrame) < 0)
									{
										printf("Error occurred when putting frame into filter context\n");
										break;
									}
									while (1)
									{
										// Get frame from filter, if it returns < 0 then filter is currently empty.
										if (av_buffersink_get_frame(filter_ctx->sink_ctx, filtered_frame3) < 0)
										{
											break;
										}
									} // while
									  //효과 필터 적용 시점

									RGBImage* frame2;
									frame2 = new RGBImage(pDlg->dst_width, pDlg->dst_height);
									frame2->copyFrame(filtered_frame3);

									u_char s_R, s_G, s_B;
									u_char d_R, d_G, d_B;
									u_char r_R, r_G, r_B;

									int H = pDlg->dst_height;
									int i_H = pDlg->img_height;

									int W = filtered_frame3->linesize[0];
									//	int i_W = pDlg->m_picture_frame->linesize[0];
									double calc_next = (double)W / (double)150;

									for (int i = 0; i < i_H; i++) {
										for (int j = 0; j <W; j++) {
											//int i_index = i*i_W + j;
											int f_index = (i)*W + j;
											//frame->getPixelColor(s_R,s_G,s_B,)
											frame->setPixelColor(0, 0, 0, f_index);

										}
									}
									for (int i = 0; i < i_H; i++) {
										for (int j = 0; j <W; j++) {
											//int i_index = i*i_W + j;
											int f_index = (i)*W + j;

											frame->setPixelColor(0, 0, 0, f_index);

										}
									}

									good = true;
									back++;
									delete frame2;
									av_frame_unref(filtered_frame3);
								}
							}
						}
						av_free_packet(&read_pkt);
					}
				}
			
			}
		}

		if (s_f) {//
			if (av_read_frame(pDlg->test_media[select_origin].ifmt_ctx, &read_pkt) >= 0) {
				///> Decoding
				//av_read_frame(a_ifmt_ctx, &apkt);
				if (read_pkt.stream_index == pDlg->nVSI) {
					i++;
					pDlg->m_watch_nbframe = i;
					pDlg->PostMessage(UM_UPDATEDAMSGFALSE, 0, 0);
					if (avcodec_decode_video2(pDlg->test_media[select_origin].dec_ctx, pVFrame, &bGotPicture, &read_pkt) >= 0) {
						if (bGotPicture) {
							FilterContext* filter_ctx = &pDlg->vfilter_ctx;
							out_pkt.data = NULL;    // packet data will be allocated by the encoder
							out_pkt.size = 0;
							if (av_buffersrc_add_frame(filter_ctx->src_ctx, pVFrame) < 0)
							{
								printf("Error occurred when putting frame into filter context\n");
								break;
							}
							while (1)
							{
								// Get frame from filter, if it returns < 0 then filter is currently empty.
								if (av_buffersink_get_frame(filter_ctx->sink_ctx, filtered_frame) < 0)
								{
									break;
								}
							} // while
							  //효과 필터 적용 시점

							RGBImage* frame3;
							frame3 = new RGBImage(pDlg->dst_width, pDlg->dst_height);
							frame3->copyFrame(filtered_frame);

							u_char s_R, s_G, s_B;
							u_char d_R, d_G, d_B;
							u_char r_R, r_G, r_B;

							int H = pDlg->dst_height;
							int i_H = pDlg->img_height;

							int W = filtered_frame->linesize[0];
						//	int i_W = pDlg->m_picture_frame->linesize[0];

							
							for (int i = 0; i < i_H; i++) {
								for (int j = 0; j <W; j++) {
									//int i_index = i*i_W + j;
									int f_index = (i)*W + j;
									double s_m = ((double)frame3->getYdata(f_index) / 255);
									double d_m = 1 - s_m;

									frame->getPixelColor(s_R, s_G, s_B, f_index);
									frame3->getPixelColor(d_R, d_G, d_B, f_index);
									r_R = (s_R*d_m + d_R*s_m);
									r_B = (s_B*d_m + d_B*s_m);
									r_G = (s_G*d_m + d_G*s_m);
									frame3->setPixelColor(r_R, r_G, r_B, f_index);
								}
							}
							frame3->RGB2YUV(*filtered_frame);
							s_f = false;
							d_f = false;
							
							
							/*
							CDC* p = m_picture_control.GetWindowDC();
							u_char R, G, B;
							for (int i = 0; i < 300; i++) {
							for (int j = 0; j < 500; j++) {
							CPoint temp(j, i);
							frame2->getPixelColor(R, G, B, i*filtered_frame->linesize[0] + j);
							p->SetPixelV(temp, RGB(R, G, B));
							}
							}
							m_picture_control.ReleaseDC(p);
							*/
							delete frame3;
							delete frame;//힙오류
							avcodec_encode_video2(pDlg->test_media[0].enc_codec_ctx, &out_pkt, filtered_frame, &bGotPicture);

						
							if (bGotPicture) {
								pDlg->debugging_count++;
								if (pDlg->debugging_count > 30) {
									pDlg->debugging_count = 0;
									FilterContext* filter_ctx_d = &pDlg->vfilter_ctx_d;

									if (av_buffersrc_add_frame(filter_ctx_d->src_ctx, filtered_frame) < 0)
									{
										printf("Error occurred when putting frame into filter context\n");
										break;
									}
									while (1)
									{
										// Get frame from filter, if it returns < 0 then filter is currently empty.
										if (av_buffersink_get_frame(filter_ctx_d->sink_ctx, pVFrame) < 0)
										{
											break;
										}
									} // while
									//pVFrmae 그려주기
									/*
									pDlg->deg_linewidth = pVFrame->linesize[0];
									pDlg->deg_Frame = new RGBImage(500, 300);
									pDlg->deg_Frame->copyFrame(pVFrame);
									//make Thread
									CWinThread *p2 = NULL;
									p2 = AfxBeginThread(ThreadSecondData, 0);

									if (p2 == NULL)
									{
										AfxMessageBox(L"Error");
									}*/
								}
								now_pts += 30*100;
								out_pkt.pts = now_pts;
								out_pkt.dts = now_pts;
								av_interleaved_write_frame(pDlg->ofmt_ctx, &out_pkt);
							//	Mat mat(filtered_frame->height, filtered_frame->width, CV_8UC3, filtered_frame->data[0], filtered_frame->linesize[0]);
							//	imshow("frame", mat);
								//waitKey(10);
								pDlg->m_progress.SetPos(i);
								N++;
							}
							av_frame_unref(filtered_frame);
						}
					}
				}
				av_free_packet(&read_pkt);
			}
			//
		}
	}
	/*
	int nframe = 0;
	text = _T("encoding Audio..");
	pDlg->m_status_text.SetWindowText(text);
	while (av_read_frame(pDlg->a_ifmt_ctx, &read_pkt) >= 0) {
		if (read_pkt.stream_index == 0) {
			if (read_pkt.data != NULL) {
				av_interleaved_write_frame(pDlg->ofmt_ctx, &out_pkt);
				av_free_packet(&out_pkt);
				if (avcodec_decode_audio4(dec_A_ctx, pAFrame, &bGotSound, &read_pkt) >= 0) {
					if (bGotSound) {
						avcodec_encode_audio2(A_enc_codec_ctx, &out_pkt, pAFrame, &bGotSound);
						if (bGotSound) {
							out_pkt.stream_index = 1;
							out_pkt.pts = out_pkt.pts / 330;
							out_pkt.dts = out_pkt.dts / 330;
							av_interleaved_write_frame(pDlg->ofmt_ctx, &out_pkt);
							nframe++;
							if (nframe > 31 * 396 / 10) {//시간 
								break;
							}
							av_free_packet(&out_pkt);
						}
					}
				}
			}
		}
		av_free_packet(&read_pkt);
	}*/
	av_write_trailer(pDlg->ofmt_ctx);
	avcodec_close(V_enc_codec_ctx);
	av_free(V_enc_codec_ctx);
	av_free(pVFrame);
	av_free(pAFrame);
	///> Close an opened input AVFormatContext. 
	avformat_close_input(&pDlg->ifmt_ctx);
	avformat_close_input(&pDlg->ofmt_ctx);
	text = _T("Done!!");
	pDlg->m_status_text.SetWindowText(text);
	CString strPath = _T("C:\\Users\\itaDev1\\Documents\\Visual Studio 2015\\Projects\\ProjectEve\\ProjectEve\\output.mp4");
	ShellExecute(NULL, _T("open"), _T("explorer"), _T("/select,") + strPath, NULL, SW_SHOW);
	return 0;
}
UINT CProjectEveDlg::ThreadThirdData(LPVOID _mothod) {
	//새로운 테스트
	return 0;
}
UINT CProjectEveDlg::ThreadSecondData(LPVOID _mothod) {

	CProjectEveDlg *fir = (CProjectEveDlg*)_mothod;
	CProjectEveDlg* pDlg = (CProjectEveDlg*)AfxGetApp()->m_pMainWnd;

	CDC* p = pDlg->m_picture_control.GetWindowDC();
	u_char R, G, B;

	for (int i = 0; i < 300; i++) {
		for (int j = 0; j < 500; j++) {
			CPoint temp(j, i);
			pDlg->deg_Frame->getPixelColor(R, G, B, i*pDlg->deg_linewidth + j);
			p->SetPixelV(temp, RGB(R, G, B));
		}
	}
	pDlg->m_picture_control.ReleaseDC(p);

	return 0;
}

void CProjectEveDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true);
	dst_width = m_scale_width;
	dst_height = m_scale_height;
	CWinThread *p1 = NULL;
	p1 = AfxBeginThread(ThreadFirstData, this);

	if (p1 == NULL)
	{
		AfxMessageBox(L"Error");
	}
	//CloseHandle(p1);
}
