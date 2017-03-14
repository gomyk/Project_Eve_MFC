#pragma once
#include <libavformat/avformat.h>

typedef uint8_t u_char;

struct data8 {
	u_char* Rdata;
	u_char* Gdata;
	u_char* Bdata;
	u_char* Ydata;
};
class RGBImage {
private:
	int width;
	int height;
	data8 buffer;
public:
	RGBImage();
	RGBImage(int width, int height);
	~RGBImage();
	bool isGood();
	void copyFrame(AVFrame* frame);
	u_char getYdata(int index);
	bool checkPixelWhite(int index);
	void getPixelColor(u_char& R, u_char& G, u_char& B, int index);
	void setPixelColor(u_char R, u_char G, u_char B, int index);
	void RGB2YUV(AVFrame& frame);
};
