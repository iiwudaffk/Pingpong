////////////////////////////////////////////////////
// project_pingpong_function.cpp
// @Author 52363202 Aekasit Nakarad
// @Author 52363219 Aopat Gaewta
// @Author 52362564 Choawarin Praserdsook
// @note VS C++ 2010
// @note OpenCV 2.4.3
// @since 2012 Feb 11
// version 1.0
////////////////////////////////////////////////////

#include "project_pingpong_function.h"
using namespace std;



float calDistance(cv::Point p1,cv::Point p2)
{
	return abs((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}

int findBiggestContour(vector<vector<cv::Point>> &contours)
{
	int index=0;
	int max_size = 0;
	int size;
	for( int i = 0; i < contours.size(); i++ )
	{
		size = cv::arcLength(contours[i], true);
		if(size > max_size)
		{
			max_size = size;
			index = i;
		}
	}

	return index;
}

float findHueInArea(cv::Mat &input, cv::Point input_point, int radius)
{
	cv::Rect roi_rect = cv::Rect((input_point.x-radius,input_point.x+radius), (input_point.y-radius,input_point.y+radius), radius, radius);
	cv::Mat temp = input(roi_rect);
	std::vector<cv::Mat> imgHSV;
	cv::split(temp, imgHSV);
	float meanHue = cv::mean(imgHSV[0])[0];

	return meanHue;
}




Ball::Ball()
{
	location = cv::Point(320,240);
	angle = 0.0;
	mode = 0;
	movex = 1;
	movey = 1;
	speed = 10;
};

cv::Point Ball::getLocation()
{
	return this->location;
}

float Ball::getAngle()
{
	return this->angle;
}

int Ball::getMode()
{
	return this->mode;
}

int Ball::getMoveX()
{
	return this->movex;
}

int Ball::getMoveY()
{
	return this->movey;
}

float Ball::getSpeed()
{
	return this->speed;
}

void Ball::setLocation()
{
	location.x = 320;
	location.y = 240;
}

void Ball::setLocation(float x,float y)
{
	location.x = x;
	location.y = y;
}

void Ball::setAngle()
{
	angle = 0.0;
}

void Ball::setAngle(float newAngle)
{
	angle = newAngle;
}

void Ball::setMode(int newMode)
{
	mode = newMode;
}

void Ball::setMoveX(int n)
{
	movex = n;
}

void Ball::setMoveY(int n)
{
	movey = n;
}

void Ball::setSpeed()
{
	speed = 10;
}

void Ball::setSpeed(float s)
{
	speed = s;
}

void Ball::drawBall(cv::Mat drawOutput)
{
	cv::circle(drawOutput, this->location, 15, cv::Scalar(0, 0, 255), 40,1);
}

void Ball::refreshBall()
{
	location = cv::Point(320,240);
	angle = 0.0;
	mode = 0;
	movex = 1;
	movey = 1;
	speed = 10;
}




Player::Player(cv::Point pos)
{
	location = pos;
	speed = 0;
	lastLocation.push(pos);
	lastLocation.push(pos);
	lastLocation.push(pos);
	lastLocation.push(pos);
	lastLocation.push(pos);
};

cv::Point Player::getLocation()
{
	return this->location;
}

float Player::getSpeed()
{
	return this->speed;
}

void Player::setLocation()
{
	location.x = 0;
	location.y = 0;
}

void Player::setLocation(cv::Point newLocation)
{
	location.x = newLocation.x;
	location.y = newLocation.y;
}

void Player::updateLast5()
{
	lastLocation.push(location);
	while(lastLocation.size()>5)
	{
		lastLocation.pop();
	}
}

void Player::calSpeed()
{
	queue<cv::Point>last5 = lastLocation;
	int i = 0;
	int a = lastLocation.size();
	float s = 0;
	cv::Point p1 = 0;
	cv::Point p2 = 0;

	while(i < a-1)
	{
		p1 = last5.front();
		last5.pop();
		p2 = last5.front();
		s = s + calDistance(p1,p2);
		i++;
	}
	//cout<<"s : "<<s<<endl;

	if(s<100)
	{
		speed = 2;
	}
	else if(s>=100 && s<300)
	{
		speed = 5;
	}
	else if(s>=300 && s<700)
	{
		speed = 8;
	}
	else if(s>=700 && s<1500)
	{
		speed = 11;
	}
	else if(s>=1500 && s<2500)
	{
		speed = 15;
	}
	else if(s>=2500 && s<5000)
	{
		speed = 20;
	}
	else if(s>=5000)
	{
		speed = 25;
	}
}

void Player::drawPlayer(cv::Mat drawOutput,int colorB,int colorG,int colorR)
{
	cv::rectangle(drawOutput, cv::Point(location.x-60,location.y-60), cv::Point(location.x+60,location.y+60), cv::Scalar(colorB, colorG, colorR), 5);
}




cv::Mat getFrameColor(cv::Mat &input,int hmin,int smin,int vmin,int hmax,int smax,int vmax)
{
	cv::Mat output = cv::Mat::zeros(cv::Size(input.size()), CV_8UC3);
	cv::Mat blob = cv::Mat::zeros(cv::Size(input.size()), CV_8UC1);

	std::vector<cv::Mat> hsvChannels(3);
	cv::Mat hsvImage;
	cv::Mat object;
	std::vector<std::vector<cv::Point>> mContours;

	cv::cvtColor(input, hsvImage, CV_BGR2HSV);

	cv::inRange(hsvImage, cv::Scalar(hmin, smin, vmin), cv::Scalar(hmax, smax, vmax), object);

	int morph_size = 4;
	cv::Mat element = cv::getStructuringElement( 2, cv::Size( 2*morph_size + 1, 2*morph_size+1 ), cv::Point( morph_size, morph_size ) );
	cv::morphologyEx( object, object, cv::MORPH_OPEN, element );	

	cv::medianBlur(object, object, 5);

	cv::findContours( object, mContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

	//// eliminate small contours
	int cmin = 100;
	std::vector<std::vector<cv::Point>>::const_iterator itc = mContours.begin();
	while (itc!=mContours.end()) 
	{
		if (itc->size() < cmin)
			itc= mContours.erase(itc);
		else
			++itc;
	}

	/// Get the moments
	int min_area = 0;
	float compactness;
	int index = 0;
	float closest = 99999;
	std::vector<cv::Moments> mu(mContours.size());
	std::vector<cv::Point2f> mc(mContours.size());
	std::vector<bool> isSelected(mContours.size());
	for( int i = 0; i< mContours.size(); i++ )
	{
		mu[i] = moments( mContours[i], false ); 
		mc[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
		//compactness = cv::arcLength( mContours[i], true );
		//compactness = compactness*compactness/mu[i].m00;
		if(mu[i].m00 > min_area)
		{
			isSelected[i] = true;
			//cv::drawContours( blob, mContours, i, cv::Scalar( 255 ), -1);
			//cv::drawContours( output, mContours, i, cv::Scalar( 255,255,255 ), -1);
			//cv::putText(output, toStrMaxDecimals(compactness,2), cvPoint(mc[i].x,mc[i].y), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0,0,250), 1, CV_AA);
			//if(fabs(compactness-4.0*CV_PI) < closest)
			//{
			//	closest = fabs(compactness-4.0*CV_PI);
			//	index = i;
			//}
		}else
		{
			isSelected[i] = false;
		}
	}

	int index_big = findBiggestContour(mContours);
	
	if((mContours.size() == 0)||(!isSelected[index_big])||(closest > 10))
	{
		/*result = cv::Point(-10,-10);*/
		//return -1;
	}else
	{
		cv::drawContours( blob, mContours, index_big, cv::Scalar( 255 ), -1);
		cv::addWeighted(object, 1.0, blob, 1.0, 0.0, blob);
	}

	return blob;
}

PointHue getPlayerLocation(cv::Mat &input,int hmin,int smin,int vmin,int hmax,int smax,int vmax)
{
	cv::Point p1 = cv::Point(20,240);
	cv::Point p2 = cv::Point(640,240);

	cv::Mat output = cv::Mat::zeros(cv::Size(input.size()), CV_8UC3);
	cv::Mat blob = cv::Mat::zeros(cv::Size(input.size()), CV_8UC1);

	std::vector<cv::Mat> hsvChannels(3);
	cv::Mat hsvImage;
	cv::Mat object;
	std::vector<std::vector<cv::Point>> mContours;

	cv::cvtColor(input, hsvImage, CV_BGR2HSV);

	cv::inRange(hsvImage, cv::Scalar(hmin, smin, vmin), cv::Scalar(hmax, smax, vmax), object);

	int morph_size = 4;
	cv::Mat element = cv::getStructuringElement( 2, cv::Size( 2*morph_size + 1, 2*morph_size+1 ), cv::Point( morph_size, morph_size ) );
	cv::morphologyEx( object, object, cv::MORPH_OPEN, element );	

	cv::medianBlur(object, object, 5);

	cv::findContours( object, mContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

	//// eliminate small contours
	int cmin = 100;
	std::vector<std::vector<cv::Point>>::const_iterator itc = mContours.begin();
	while (itc!=mContours.end()) 
	{
		if (itc->size() < cmin)
			itc= mContours.erase(itc);
		else
			++itc;
	}

	/// Get the moments
	int min_area = 0;
	float compactness;
	//int index = 0;
	float closest = 8;
	//float closest = 8;
	std::vector<cv::Moments> mu(mContours.size());
	std::vector<cv::Point2f> mc(mContours.size());
	std::vector<bool> isSelected(mContours.size());
	for( int i = 0; i< mContours.size(); i++ )
	{
		mu[i] = moments( mContours[i], false ); 
		mc[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
		//compactness = cv::arcLength( mContours[i], true );
		//compactness = compactness*compactness/mu[i].m00;
		if(mu[i].m00 > min_area)
		{
			isSelected[i] = true;
			//cv::putText(output, toStrMaxDecimals(compactness,2), cvPoint(mc[i].x,mc[i].y), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0,0,250), 1, CV_AA);
			//if(fabs(compactness-4.0*CV_PI) < closest)
			//{
			//	closest = fabs(compactness-4.0*CV_PI);
			//	index = i;
			//}
		}else
		{
			isSelected[i] = false;
		}
	}

	// find Biggest Contour
	int index_big = findBiggestContour(mContours);

	PointHue result;
	
	if((mContours.size() == 0)||(!isSelected[index_big])/*||(closest > 10)*/)
	{
		result.hue = 0;
		result.point_cen = cv::Point(-100,-100);
	}else
	{
		// find average hue around the centroid
		int rect_size = 20;
		cv::Rect roi_rect = cv::Rect(mc[index_big].x - rect_size/2, mc[index_big].y - rect_size/2, rect_size, rect_size);
	
		cv::Mat roiImg;

		roiImg = hsvImage(roi_rect);
		std::vector<cv::Mat> imgHSV;
		cv::split(roiImg, imgHSV);
		float meanHue = cv::mean(imgHSV[0])[0];
		//meanHue = meanHue/180.0;

		//cout<<"This MeanHue : "<<meanHue<<endl;
		result.hue = meanHue;
		result.point_cen = cv::Point(mc[index_big].x,mc[index_big].y);
	}
	return result;
}
