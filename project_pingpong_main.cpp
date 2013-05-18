////////////////////////////////////////////////////
// project_pingpong_main.cpp
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
#include "project_pingpong_function.h"
using namespace std;

cv::RNG rng(12345);

int main()
{
	// Open the video file
	cv::VideoCapture capture(0);
	// check if video successfully opened
	if (!capture.isOpened())
		return 1;
	// Get the frame rate
	double rate= capture.get(CV_CAP_PROP_FPS);
	long frame_no = static_cast<long>(capture.get(CV_CAP_PROP_FRAME_COUNT));
	bool stop(false);

	cv::Mat frame; // current video frame
	//cv::namedWindow("Extracted Frame");
	cv::Size size = cv::Size((int) capture.get(CV_CAP_PROP_FRAME_WIDTH),    
		(int) capture.get(CV_CAP_PROP_FRAME_HEIGHT));

	cv::Mat output = cv::Mat::zeros(cv::Size(frame.size()), CV_8UC3);
	//cv::Mat output2 = cv::Mat::zeros(cv::Size(frame.size()), CV_8UC3);
	//cv::Mat output3 = cv::Mat::zeros(cv::Size(frame.size()), CV_8UC3);

	// create initial value program 
	Ball MyBall;
	Player PlayerLeft(cv::Point(20,0));
	Player PlayerRight(cv::Point(620,0));
	Player GoalL(cv::Point(20,240));
	Player GoalR(cv::Point(620,240));
	int pl_score = 0;
	int pr_score = 0;
	bool gameFinish = false;
	
	
	// for all frames in video
	while (!stop) 
	{
		// read next frame if any
		if (!capture.read(frame))
			break;

		cv::flip(frame, frame, 1);
		frame.copyTo(output);
		//frame.copyTo(output2);

		//cout<<"Center MeanHue Left : "<<findHueInArea(frame,GoalL.getLocation(),40)<<endl;
		//cout<<"Center MeanHue Right : "<<findHueInArea(frame,GoalR.getLocation(),40)<<endl;
		GoalL.drawPlayer(output,0,255,255);
		GoalR.drawPlayer(output,0,255,255);
		cv::line(output,cv::Point(320,0),cv::Point(320,480),cv::Scalar(0,255,255),2);

		//(10,30) orange
		//(35,85) green
		//(70,110) blue
		//(120,172) pink

		// get player's location
		PointHue pl = getPlayerLocation(frame,70,100,100,110,255,255);
		if(pl.point_cen.x > 320) pl.point_cen.x = 320;
		PointHue pr = getPlayerLocation(frame,120,100,100,172,255,255);
		if(pr.point_cen.x < 320) pr.point_cen.x = 320;

		PlayerLeft.setLocation(pl.point_cen);
		PlayerLeft.updateLast5();
		PlayerLeft.calSpeed();
		PlayerLeft.drawPlayer(output,0,255,0);
		
		PlayerRight.setLocation(pr.point_cen); 
		PlayerRight.updateLast5();
		PlayerRight.calSpeed();
		PlayerRight.drawPlayer(output,255,0,255);
		
		MyBall.drawBall(output);
		
		// calculate distance between Ball and Player
		float dp1 = calDistance(MyBall.getLocation(),PlayerLeft.getLocation());
		float dp2 = calDistance(MyBall.getLocation(),PlayerRight.getLocation());

		// calculate distance between Ball and Goal
		float dgl = calDistance(MyBall.getLocation(),GoalL.getLocation());
		float dgr = calDistance(MyBall.getLocation(),GoalR.getLocation());

		// calculate distance between Player and Goal
		float dglp1 = calDistance(PlayerLeft.getLocation(),GoalL.getLocation());
		float dgrp2 = calDistance(PlayerRight.getLocation(),GoalR.getLocation());

		cout<<"###############################################"<<endl;

		cout<<"Player Left x : "<<PlayerLeft.getLocation().x<<endl;
		cout<<"Player Left y : "<<PlayerLeft.getLocation().y<<endl;
		cout<<"Player Left Speed : "<<PlayerLeft.getSpeed()<<endl;
		cout<<"Player Left MeanHue : "<<pl.hue<<endl;

		cout<<"Player Right x : "<<PlayerRight.getLocation().x<<endl;
		cout<<"Player Right y : "<<PlayerRight.getLocation().y<<endl;
		cout<<"Player Right Speed : "<<PlayerRight.getSpeed()<<endl;
		cout<<"Player Right MeanHue : "<<pr.hue<<endl;

		cout<<"MyBall x : "<<MyBall.getLocation().x<<endl;
		cout<<"MyBall y : "<<MyBall.getLocation().y<<endl;

		cout<<"Distance P1 and Ball : "<<dp1<<endl;
		cout<<"Distance P2 ans Ball : "<<dp2<<endl;

		cout<<"Distance Goal Left and Ball : "<<dgl<<endl;
		cout<<"Distance Goal Right and Ball : "<<dgr<<endl;

		cout<<"PlayerLeft Distance Goal Left : "<<dglp1<<endl;
		cout<<"PlayerRight Distance Goal Right : "<<dgrp2<<endl;

		cout<<"MyBall Mode : "<<MyBall.getMode()<<endl;
		cout<<"MyBall Speed : "<<MyBall.getSpeed()<<endl;

		cout<<"###############################################"<<endl;
		cout<<"################### Control ###################"<<endl;
		cout<<"Press  +  to Speed Up Ball"<<endl;
		cout<<"Press  -  to Speed Down Ball"<<endl;
		cout<<"Press  s  to Stop Ball"<<endl;
		cout<<"Press  r  to Refresh Ball"<<endl;
		cout<<"Press  p  to Reset Score"<<endl;
		cout<<"Press  Esc  to Exit Program"<<endl;
		cout<<"###############################################"<<endl;


		if(!gameFinish)
		{

			// check ball and player to set motion mode
			if(dp1<=8000 || dp2<=8000)
			{
				MyBall.setMode(1);
				if(PlayerLeft.getLocation().x <= MyBall.getLocation().x && dp1<dp2)
				{
					MyBall.setMoveX(1);
					MyBall.setSpeed(MyBall.getSpeed() + PlayerLeft.getSpeed());
				}
				else if(PlayerLeft.getLocation().x > MyBall.getLocation().x && dp1<dp2)
				{
					MyBall.setMoveX(0);
					MyBall.setSpeed(MyBall.getSpeed() + PlayerLeft.getSpeed());
				}
				else if(dp2<dp1 && PlayerRight.getLocation().x < MyBall.getLocation().x)
				{
					MyBall.setMoveX(1);
					MyBall.setSpeed(MyBall.getSpeed() + PlayerRight.getSpeed());
				}
				else if(dp2<dp1 && PlayerRight.getLocation().x >= MyBall.getLocation().x)
				{
					MyBall.setMoveX(0);
					MyBall.setSpeed(MyBall.getSpeed() + PlayerRight.getSpeed());
				}
			}

			// show motion and check Ball mode
			if(MyBall.getMode()==1)
			{
				if(MyBall.getMoveX()==1 && MyBall.getMoveY()==1)//(+,+)
				{
					MyBall.setLocation(MyBall.getLocation().x+MyBall.getSpeed(),MyBall.getLocation().y+MyBall.getSpeed());
					if(MyBall.getLocation().x>=640)
					{
						MyBall.setMoveX(0);
						MyBall.setSpeed(MyBall.getSpeed() - 5);
						if(MyBall.getSpeed() <= 10)
						{
							MyBall.setSpeed();
						}
					}
					if(MyBall.getLocation().y>=480)
					{
						MyBall.setMoveY(0);
						MyBall.setSpeed(MyBall.getSpeed() - 5);
						if(MyBall.getSpeed() <= 10)
						{
							MyBall.setSpeed();
						}
					}
				}
				else if(MyBall.getMoveX()==0 && MyBall.getMoveY()==1)//(-,+)
				{
					MyBall.setLocation(MyBall.getLocation().x-MyBall.getSpeed(),MyBall.getLocation().y+MyBall.getSpeed());
					if(MyBall.getLocation().x<=0)
					{
						MyBall.setMoveX(1);
						MyBall.setSpeed(MyBall.getSpeed() - 5);
						if(MyBall.getSpeed() <= 10)
						{
							MyBall.setSpeed();
						}
					}
					if(MyBall.getLocation().y>=480)
					{
						MyBall.setMoveY(0);
						MyBall.setSpeed(MyBall.getSpeed() - 5);
						if(MyBall.getSpeed() <= 10)
						{
							MyBall.setSpeed();
						}
					}
				}
				else if(MyBall.getMoveX()==0 && MyBall.getMoveY()==0)//(-,-)
				{
					MyBall.setLocation(MyBall.getLocation().x-MyBall.getSpeed(),MyBall.getLocation().y-MyBall.getSpeed());
					if(MyBall.getLocation().x<=0)
					{
						MyBall.setMoveX(1);
						MyBall.setSpeed(MyBall.getSpeed() - 5);
						if(MyBall.getSpeed() <= 10)
						{
							MyBall.setSpeed();
						}
					}
					if(MyBall.getLocation().y<=0)
					{
						MyBall.setMoveY(1);
						MyBall.setSpeed(MyBall.getSpeed() - 5);
						if(MyBall.getSpeed() <= 10)
						{
							MyBall.setSpeed();
						}
					}
				}
				else if(MyBall.getMoveX()==1 && MyBall.getMoveY()==0)//(+,-)
				{
					MyBall.setLocation(MyBall.getLocation().x+MyBall.getSpeed(),MyBall.getLocation().y-MyBall.getSpeed());
					if(MyBall.getLocation().x>=640)
					{
						MyBall.setMoveX(0);
						MyBall.setSpeed(MyBall.getSpeed() - 5);
						if(MyBall.getSpeed() <= 10)
						{
							MyBall.setSpeed();
						}
					}
					if(MyBall.getLocation().y<=0)
					{
						MyBall.setMoveY(1);
						MyBall.setSpeed(MyBall.getSpeed() - 5);
						if(MyBall.getSpeed() <= 10)
						{
							MyBall.setSpeed();
						}
					}
				}
			}
		}

		// check distance Ball in Goal
		if(dgl <= 4000)
		{
			pr_score += 1;
			MyBall.refreshBall();
		}
		else if(dgr <= 4000)
		{
			pl_score += 1;
			MyBall.refreshBall();
		}

		stringstream ss;
		ss << pl_score;
		string str1 = ss.str();

		stringstream ss2;
		ss2 << pr_score;
		string str2 = ss2.str();

		string text1 = "P1 Score : "  + str1;
		string text2 = "P2 Score : " + str2;

		cv::putText(output,text1,cv::Point(20,30),2,1,cv::Scalar(0,240,55),1);
		cv::putText(output,text2,cv::Point(400,30),2,1,cv::Scalar(240,0,240),1);

		if(pl_score>=5)
		{
			gameFinish=true;
			cv::putText(output,"Player 1 Win !!",cv::Point(90,200),2,2,cv::Scalar(0,240,55),3);
			cv::putText(output,"<-Press Goal To Start Game->",cv::Point(55,300),2,1,cv::Scalar(240,0,240),2);
			if(dglp1 <= 3000 || dgrp2 <= 3000)
			{
				MyBall.refreshBall();
				pl_score = 0;
				pr_score = 0;
				gameFinish=false;
			}
		}
		else if(pr_score>=5)
		{
			gameFinish=true;
			cv::putText(output,"Player 2 Win !!",cv::Point(90,200),2,2,cv::Scalar(240,0,240),3);
			cv::putText(output,"<-Press Goal To Start Game->",cv::Point(55,300),2,1,cv::Scalar(0,240,55),2);
			MyBall.refreshBall();
			if(dglp1 <= 3000 || dgrp2 <= 3000)
			{
				MyBall.refreshBall();
				pl_score = 0;
				pr_score = 0;
				gameFinish=false;
			}
		}
		
		system("cls");
		//cv::imshow("Green Chennel",getFrameColor(output,70,100,100,110,255,255));
		//cv::imshow("Pink Chennel",getFrameColor(output,120,100,100,172,255,255));
		cv::imshow("Pingpong",output);

		char c = cv::waitKey(10);

		if( c == 27 ) 
		{	break;	}
		else if (c=='r') 
		{ 	MyBall.refreshBall(); }
		else if (c=='p')
		{	pl_score = 0;
			pr_score = 0;	}
		else if (c=='s') 
		{	MyBall.setMode(0);	}
		else if (c=='+') 
		{	MyBall.setSpeed(MyBall.getSpeed()+5);	}
		else if (c=='-') 
		{	MyBall.setSpeed(MyBall.getSpeed()-5);	}
		
	}
	// Close the video file.
	// Not required since called by destructor

	capture.release();

	cv::destroyAllWindows();
}
