#pragma once
class MovingImage {
private:
	CPoint to;
	CPoint from;
	CPoint current;
	int image_index;
	int count;
	bool left;
	int speed;
	int range;
	int is_curved;
	int was_curved;
	int sum;
	int frame_num;
	double up_speed;
public:
	MovingImage();
	MovingImage(CPoint from, CPoint to);
	~MovingImage();

	void SetToPos(CPoint data);
	void SetFromPos(CPoint data);
	void SetImage(int index);
	int GetImageIndex();
	CPoint GetCurrentPosition();
	void calcNextPosition();
	void goup();
	bool ok2go();
};