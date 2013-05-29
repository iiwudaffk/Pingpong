////////////////////////////////////////////////////
// project_pingpong_function.h
// @Author 52363202 Aekasit Nakarad
// @Author 52363219 Aopat Gaewta
// @Author 52362564 Choawarin Praserdsook
// @note VS C++ 2010
// @note OpenCV 2.4.3
// @since 2012 Feb 11
// version 1.0
////////////////////////////////////////////////////

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <fstream>
#include <queue>
using namespace std;

struct PointHue
{
	cv::Point point_cen;
	float hue;
};

class Ball
{
private:
	cv::Point location;
	cv::Point screenCenter;
	float angle;
	int mode;// 0=stop, 1=move
	int movex;// 0= --, 1= ++
	int movey;// 0= --, 1= ++
	float speed;
public:
	cv::Point getLocation();
	float getAngle();
	int getMode();
	int getMoveX();
	int getMoveY();
	float getSpeed();

	void setLocation();
	void setLocation(float,float);
	void setAngle();
	void setAngle(float);
	void setMode(int);
	void setMoveX(int);
	void setMoveY(int);
	void setSpeed();
	void setSpeed(float);

	void drawBall(cv::Mat);
	void refreshBall();

	// default constructor
	Ball(int screenWidth,int screenHeight);
};

class Player
{
private:
	cv::Point location;
	queue<cv::Point>lastLocation;
	float speed;
public:
	cv::Point getLocation();
	float getSpeed();
	
	void setLocation();
	void setLocation(cv::Point);
	void updateLast5();
	void calSpeed();
	void drawPlayer(cv::Mat&,int,int,int);
	
	// default constructor
	Player(cv::Point pos);
};

// calculate distance between 2 points
// @return distance between 2 points(float)
float calDistance(cv::Point p1,cv::Point p2);

// find biggest index in contours
// @return index of biggest contour(int)
int findBiggestContour(vector<vector<cv::Point>> &contours);

// calculate Hue color value in some area
// @return hue(float) (min=0,max=180)
float findHueInArea(cv::Mat &input, cv::Mat &output,cv::Point input_point, int radius);

// threshold image in HSV range
// @return image as threshold(cv::Mat)
cv::Mat getFrameColor(cv::Mat &input,int hmin,int smin,int vmin,int hmax,int smax,int vmax);

// find centroid of biggest contour in HSV range
// @return centroid of biggest contour(cv::Point)
PointHue getPlayerLocation(cv::Mat &input,int hmin,int smin,int vmin,int hmax,int smax,int vmax);


// nomalize color BGR
// @return image 3 channels(cv::Mat)
cv::Mat normalizeColor(cv::Mat &input);
