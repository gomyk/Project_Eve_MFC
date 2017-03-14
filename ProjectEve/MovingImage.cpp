#include "stdafx.h"
#include "MovingImage.h"
#define PI 3.14159265
MovingImage::MovingImage() {
	count = 0;
}
MovingImage::MovingImage(CPoint from,CPoint to) {
	this->from = from;
	this->to = to;
	srand(time(NULL));
}
MovingImage::~MovingImage() {

}
void MovingImage::SetToPos(CPoint data) {
	to = data;
}
void MovingImage::SetFromPos(CPoint data) {
	from = data;
	current = data;
	speed = rand() % 10 + 2;
	range = rand() % 10 + 2;
	is_curved = 1;
	was_curved = 0;
	sum = 0;
}
void MovingImage::SetImage(int index) {
	image_index = index;
}
int MovingImage::GetImageIndex() {
	return image_index;
}
CPoint MovingImage::GetCurrentPosition() {
	return current;
}
void MovingImage::calcNextPosition() {//수정필요 특정위치에서 0,0위치로만 됨
	if (to.x == current.x) {
		CPoint temp = to;
		current = to;
		to = from;
		from = temp;
		//setnext
		return;
	}
	if (to.x - current.x < 0) {
		current.x--;
	}
	else {
		current.x++;
	}
	if (to.y - current.y < 0) {
		current.y = (int)(double(((current.x)*(double)((double)(from.y - to.y) / (double)(from.x - to.x))) + 0.5));
	}
	else {
		current.y = (int)(double(((current.x)*(double)((double)(from.y - to.y) / (double)(from.x - to.x))) + 0.5));
	}
}
void MovingImage::goup() {
	
	if (current.y == 0) {
		current = from;
		return;
	}
	current.y--;
	double result;
	sum += speed;
	
	result = range * sin(sum* PI / 180) + 0.5;
	if (result>= 0 && is_curved > 0) {

	}
	else if (result < 0 && is_curved < 0) {

	}
	else {
		//바뀐순간
		speed = rand() % (10) + 2;
		range = rand() % 10 + 2;
		is_curved *= -1;
	}
	
	current.x += (int)result;
	if (current.x < 0) {
		current.x = 0;
	}
	count++;
}
bool MovingImage::ok2go() {
	return true;
}