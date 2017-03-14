#include "stdafx.h"
#include "RGBImage.h"
RGBImage::RGBImage() {

}
RGBImage::RGBImage(int width, int height) {
	this->width = width;
	this->height = height;
}
RGBImage::~RGBImage() {
	delete[] buffer.Rdata;
	delete[] buffer.Gdata;
	delete[] buffer.Bdata;
	delete[] buffer.Ydata;
}
bool RGBImage::isGood() {
	if (width > 0) {
		return true;
	}
	else {
		return false;
	}
}
void RGBImage::copyFrame(AVFrame* frame) {
	width = frame->width;
	height = frame->height;
	int H = frame->height;
	int W = frame->linesize[0];
	buffer.Rdata = new u_char[W*H];
	buffer.Gdata = new u_char[W*H];
	buffer.Bdata = new u_char[W*H];
	buffer.Ydata = new u_char[W*H];
	int Y, U, V;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			Y = frame->data[0][i*W + j];
			U = frame->data[1][((i / 2)*(W / 2) + j / 2)];
			V = frame->data[2][((i / 2)*(W / 2) + j / 2)];

			int R, G, B;
			//Rdata[i*W + j] = frame->data[0][i*W + j] + (1.370705 * (frame->data[2][((i / 2)*(W / 2) + j / 2)] - 128));
			//Gdata[i*W + j] = frame->data[0][i*W + j] - (0.698001 * (frame->data[2][((i / 2)*(W / 2) + j / 2)] - 128)) - (0.337633 * (frame->data[1][((i / 2)*(W / 2) + j / 2)] - 128));
			//Bdata[i*W + j] = frame->data[0][i*W + j] + (1.732446 * frame->data[1][((i / 2)*(W / 2) + j / 2)]);
			R = (int)(1.164*(Y - 16) + 1.596*(V - 128));
			G = (int)(1.164*(Y - 16) - 0.813*(V - 128) - 0.391*(U - 128));
			B = (int)(1.164*(Y - 16) + 2.018*(U - 128));
			if (R < 0) {
				R = 0;
			}
			else if (R > 255) {
				R = 255;
			}
			if (G < 0) {
				G = 0;
			}
			else if (G > 255) {
				G = 255;
			}
			if (B < 0) {
				B = 0;
			}
			else if (B > 255) {
				B= 255;
			}
			buffer.Rdata[i*W + j] = R;
			buffer.Gdata[i*W + j] = G;
			buffer.Bdata[i*W + j] = B;
			buffer.Ydata[i*W + j] = Y;
			//Rdata[i*W + j] =(int) frame->data[0][i*W + j] + 1.402*frame->data[2][((i / 2)*(W / 2) + j / 2)];
			//Gdata[i*W + j] = frame->data[0][i*W + j] - 0.344*frame->data[1][((i / 2)*(W / 2) + j / 2)] - 0.714*frame->data[2][((i / 2)*(W / 2) + j / 2)];
		//	Bdata[i*W + j] = frame->data[0][i*W + j] + 1.772*frame->data[1][((i / 2)*(W / 2) + j / 2)];
		
		}
	}
}
bool RGBImage::checkPixelWhite(int index) {
	if (buffer.Rdata[index] > 250 &&
		buffer.Gdata[index] > 250 &&
		buffer.Bdata[index] > 250 )
		return true;
	return false;
}
void RGBImage::getPixelColor(u_char& R, u_char& G, u_char& B, int index) {
	R = buffer.Rdata[index];
	G = buffer.Gdata[index];
	B = buffer.Bdata[index];
}
u_char RGBImage::getYdata(int index) {
	return buffer.Ydata[index];
}
void RGBImage::setPixelColor(u_char R, u_char G, u_char B, int index) {
	buffer.Rdata[index] = R;
	buffer.Gdata[index] = G;
	buffer.Bdata[index] = B;
}
void RGBImage::RGB2YUV(AVFrame& frame) {
	int H = height;
	int W = frame.linesize[0];
	int R, G, B;
	
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			R = buffer.Rdata[i*W + j];
			G = buffer.Gdata[i*W + j];
			B = buffer.Bdata[i*W + j];
			frame.data[0][i*W + j] = (0.257* R) + (0.504*G) + (0.098*B) + 16;
			frame.data[1][((i / 2)*(W / 2) + j / 2)] = -(0.148*R) - (0.291*G) + (0.439*B) + 128;
			frame.data[2][((i / 2)*(W / 2) + j / 2)] = (0.439*R) - (0.368*G) - (0.071*B) + 128;
		}
	}
}