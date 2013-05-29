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
#include <Windows.h>
#include <MMSystem.h>
#include "project_pingpong_function.h"
using namespace std;

cv::RNG rng(12345);
int scoreWin = 7;

int main()
{
	int winselect;
	int screenX;
	int screenY;
	cout<<"###############################################"<<endl;
	cout<<"########### Welcome Pingpong Game #############"<<endl;
	cout<<"Please setup window :"<<endl;
	cout<<"[1] 640 x 480 (4:3)"<<endl;
	cout<<"[2] 800 x 600 (4:3)"<<endl;
	cout<<"[3] 1024 x 768 (4:3)"<<endl;
	cout<<"[4] 1366 x 768 (16:9)"<<endl;
	cout<<"[5] 1280 x 1024 (5:4)"<<endl;
	cout<<"[6] 1920 x 1080 (16:9)"<<endl;
	cout<<"###############################################"<<endl;
	cin>>winselect;
	cout<<"###############################################"<<endl;

	if(winselect==1){screenX=640;screenY=460;}
	else if(winselect==2){screenX=800;screenY=580;}
	else if(winselect==3){screenX=1024;screenY=748;}
	else if(winselect==4){screenX=1366;screenY=748;}
	else if(winselect==5){screenX=1280;screenY=1004;}
	else if(winselect==6){screenX=1920;screenY=1060;}
	else {screenX=800;screenY=580;}

	// get screen resolution from system
	//screenX = GetSystemMetrics(SM_CXSCREEN);
	//screenY = GetSystemMetrics(SM_CYSCREEN);

	// Open the video file
	cv::VideoCapture capture(0);
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	

	// check if video successfully opened
	if (!capture.isOpened())
		return 1;

	//cv::Size size = cv::Size((int) capture.get(CV_CAP_PROP_FRAME_WIDTH),    
	//	(int) capture.get(CV_CAP_PROP_FRAME_HEIGHT));
	cv::Size size = cv::Size(screenX, screenY);

   	cv::Mat frame = cv::Mat::zeros(size,CV_8UC3); // current video frame
	//cv::Mat frame1 = cv::Mat::zeros(size,CV_8UC3); // current video frame
	cv::namedWindow("Pingpong");
	
	cv::Mat output = cv::Mat::zeros(cv::Size(frame.size()), CV_8UC3);
	//cv::Mat output2 = cv::Mat::zeros(cv::Size(frame.size()), CV_8UC3);
	//cv::Mat output3 = cv::Mat::zeros(cv::Size(frame.size()), CV_8UC3);
                                    
	cout<<"frame size :"<<frame.size()<<endl;
	cout<<"output size :"<<output.size()<<endl;
	cout<<"capture size :"<<capture.get(CV_CAP_PROP_FRAME_WIDTH)<<endl;
	cout<<"capture size :"<<capture.get(CV_CAP_PROP_FRAME_HEIGHT)<<endl;

	// create initial value program 
	Ball MyBall(screenX,screenY);
	Player PlayerLeft(cv::Point(20,0));
	Player PlayerRight(cv::Point(screenX-20,0));
	Player GoalL(cv::Point(20,screenY/2));
	Player GoalR(cv::Point(screenX-20,screenY/2));
	int pl_score = 0;
	int pr_score = 0;
	bool gameFinish = false;

	//PlaySound(TEXT("splash out.wav"), NULL, SND_LOOP|SND_ASYNC|SND_NOSTOP);
	
	// for all frames in video
	while (1) 
	{
		capture >> frame;
		
		// read next frame if any
		if (frame.empty())
		//if (!capture.read(frame))
			break;
		cv::flip(frame, frame, 1);
		frame.copyTo(output);
		//frame.copyTo(output2);
		
		// normalize color
		//output2 = normalizeColor(frame);
		frame = normalizeColor(frame);

		//(10,30) orange
		//(35,85) green
		//(70,110) blue
		//(120,172) pink

		// get player's location
		PointHue pl = getPlayerLocation(frame,100,100,100,130,255,255);//70 110
		PointHue pr = getPlayerLocation(frame,160,100,100,180,255,255);// 120 172
		
		// mutiple player's location by screen resolution
		if(winselect==1){screenX=640;screenY=460;pl.point_cen.y *= 1.043 /*1.0*/ ;pr.point_cen.y *= 1.043 /*1.0*/ ;}
		else if(winselect==2){pl.point_cen.x *= 1.25 ;pl.point_cen.y *= 1.25 ;pr.point_cen.x *= 1.25 ;pr.point_cen.y *= 1.25 ;}
		else if(winselect==3){screenX=1024;screenY=748;pl.point_cen.x *= 1.6 ;pl.point_cen.y *= 1.55833 /*1.6*/ ;pr.point_cen.x *= 1.6 ;pr.point_cen.y *= 1.55833 /*1.6*/ ;}
		else if(winselect==4){screenX=1366;screenY=748;pl.point_cen.x *= 2.134375 ;pl.point_cen.y *= 1.55833 /*1.6*/ ;pr.point_cen.x *= 2.134375 ;pr.point_cen.y *= 1.55833 /*1.6*/ ;}
		else if(winselect==5){screenX=1280;screenY=1004;pl.point_cen.x *= 2.0 ;pl.point_cen.y *= 2.0916667 /*2.1333*/ ;pr.point_cen.x *= 2.0 ;pr.point_cen.y *= 2.0916667 /*2.1333*/ ;}
		else if(winselect==6){screenX=1920;screenY=1060;pl.point_cen.x *= 3.0 ;pl.point_cen.y *= 2.208333 /*2.25*/ ;pr.point_cen.x *= 3.0 ;pr.point_cen.y *= 2.208333 /*2.25*/ ;}


		// set player do not offensive area of player's opponent
		if(pl.point_cen.x > screenX/2) pl.point_cen.x = screenX/2;
		if(pr.point_cen.x < screenX/2) pr.point_cen.x = screenX/2;

		// resize for show image
		cv::resize(frame,frame,size);
		cv::resize(output,output,size);

		//cout<<"Center MeanHue : "<<findHueInArea(frame,output,cv::Point(screenX/2,screenY/2),100)<<endl;
		//output2 = getFrameColor(frame,80,100,100,120,255,255);
		
		// draw net center
		cv::line(output,cv::Point(screenX/2,0),cv::Point(screenX/2,screenY+20),cv::Scalar(0,255,255),2);

		// draw Goal's location
		GoalL.drawPlayer(output,0,255,255);
		GoalR.drawPlayer(output,0,255,255);

		// set Player's location
		PlayerLeft.setLocation(pl.point_cen);
		PlayerLeft.updateLast5();
		PlayerLeft.calSpeed();
		PlayerLeft.drawPlayer(output,0,255,0);
		
		// set Player's location
		PlayerRight.setLocation(pr.point_cen); 
		PlayerRight.updateLast5();
		PlayerRight.calSpeed();
		PlayerRight.drawPlayer(output,255,0,255);
		
		// draw current ball's position
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

		//cout<<"MyBall x : "<<MyBall.getLocation().x<<endl;
		//cout<<"MyBall y : "<<MyBall.getLocation().y<<endl;

		//cout<<"Distance P1 and Ball : "<<dp1<<endl;
		//cout<<"Distance P2 ans Ball : "<<dp2<<endl;

		//cout<<"Distance Goal Left and Ball : "<<dgl<<endl;
		//cout<<"Distance Goal Right and Ball : "<<dgr<<endl;

		//cout<<"PlayerLeft Distance Goal Left : "<<dglp1<<endl;
		//cout<<"PlayerRight Distance Goal Right : "<<dgrp2<<endl;

		//cout<<"MyBall Mode : "<<MyBall.getMode()<<endl;
		//cout<<"MyBall Speed : "<<MyBall.getSpeed()<<endl;

		cout<<"###############################################"<<endl;
		cout<<"################### Control ###################"<<endl;
		cout<<"Press  +  to Speed Up Ball"<<endl;
		cout<<"Press  -  to Speed Down Ball"<<endl;
		cout<<"Press  s  to Stop Ball"<<endl;
		cout<<"Press  r  to Refresh Ball"<<endl;
		cout<<"Press  p  to Reset Score"<<endl;
		cout<<"Press  Esc  to Exit Program"<<endl;
		cout<<"###############################################"<<endl;
		//cout<<"width :"<<screenX<<" and Height :"<<screenY<<endl;
		//cout<<"frame size :"<<frame.size()<<endl;

		//GetSystemMetrics(SM_CXSCREEN);
		//GetSystemMetrics(SM_CYSCREEN);
		//SystemParametersInfo(SPI_GETWORKAREA);


		if(!gameFinish)
		{

			// check ball and player to set motion mode
			if(dp1<=8000 || dp2<=8000)
			{
				MyBall.setMode(1);
				if(PlayerLeft.getLocation().x <= MyBall.getLocation().x && dp1<dp2)
				{
					PlaySound(TEXT("sound//cartoon035_hit.wav"), NULL, SND_ASYNC);
					MyBall.setMoveX(1);
					MyBall.setSpeed(MyBall.getSpeed() + PlayerLeft.getSpeed());
				}
				else if(PlayerLeft.getLocation().x > MyBall.getLocation().x && dp1<dp2)
				{
					PlaySound(TEXT("sound//cartoon035_hit.wav"), NULL, SND_ASYNC);
					MyBall.setMoveX(0);
					MyBall.setSpeed(MyBall.getSpeed() + PlayerLeft.getSpeed());
				}
				else if(dp2<dp1 && PlayerRight.getLocation().x < MyBall.getLocation().x)
				{
					PlaySound(TEXT("sound//cartoon035_hit.wav"), NULL, SND_ASYNC);
					MyBall.setMoveX(1);
					MyBall.setSpeed(MyBall.getSpeed() + PlayerRight.getSpeed());
				}
				else if(dp2<dp1 && PlayerRight.getLocation().x >= MyBall.getLocation().x)
				{
					PlaySound(TEXT("sound//cartoon035_hit.wav"), NULL, SND_ASYNC);
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
					if(MyBall.getLocation().x>=screenX)
					{
						PlaySound(TEXT("sound//cartoon007_clang.wav"), NULL, SND_ASYNC);
						MyBall.setMoveX(0);
						MyBall.setSpeed(MyBall.getSpeed() - 5);
						if(MyBall.getSpeed() <= 10)
						{
							MyBall.setSpeed();
						}
					}
					if(MyBall.getLocation().y>=screenY)
					{
						PlaySound(TEXT("sound//cartoon007_clang.wav"), NULL, SND_ASYNC);
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
						PlaySound(TEXT("sound//cartoon007_clang.wav"), NULL, SND_ASYNC);
						MyBall.setMoveX(1);
						MyBall.setSpeed(MyBall.getSpeed() - 5);
						if(MyBall.getSpeed() <= 10)
						{
							MyBall.setSpeed();
						}
					}
					if(MyBall.getLocation().y>=screenY)
					{
						PlaySound(TEXT("sound//cartoon007_clang.wav"), NULL, SND_ASYNC);
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
						PlaySound(TEXT("sound//cartoon007_clang.wav"), NULL, SND_ASYNC);
						MyBall.setMoveX(1);
						MyBall.setSpeed(MyBall.getSpeed() - 5);
						if(MyBall.getSpeed() <= 10)
						{
							MyBall.setSpeed();
						}
					}
					if(MyBall.getLocation().y<=0)
					{
						PlaySound(TEXT("sound//cartoon007_clang.wav"), NULL, SND_ASYNC);
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
					if(MyBall.getLocation().x>=screenX)
					{
						PlaySound(TEXT("sound//cartoon007_clang.wav"), NULL, SND_ASYNC);
						MyBall.setMoveX(0);
						MyBall.setSpeed(MyBall.getSpeed() - 5);
						if(MyBall.getSpeed() <= 10)
						{
							MyBall.setSpeed();
						}
					}
					if(MyBall.getLocation().y<=0)
					{
						PlaySound(TEXT("sound//cartoon007_clang.wav"), NULL, SND_ASYNC);
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
			PlaySound(TEXT("sound//cartoon004_horn.wav"), NULL, SND_ASYNC);
			pr_score += 1;
			MyBall.refreshBall();
		}
		else if(dgr <= 4000)
		{
			PlaySound(TEXT("sound//cartoon004_horn.wav"), NULL, SND_ASYNC);
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

		// draw Player's score
		cv::putText(output,text1,cv::Point(20,50),2,2,cv::Scalar(0,240,55),2);
		cv::putText(output,text2,cv::Point(screenX/2 + 20,50),2,2,cv::Scalar(240,0,240),2);

		// check win
		if(pl_score>=scoreWin)
		{
			gameFinish=true;
			cv::putText(output,"Player 1 Win !!",cv::Point(20,screenY/3),2,2,cv::Scalar(0,240,55),3);
			cv::putText(output,"<--- Press Goal To Start Game --->",cv::Point(screenX/2 - 320,screenY/2 + 60),2,1,cv::Scalar(240,0,240),2);
			if(dglp1 <= 5000 || dgrp2 <= 5000)
			{
				MyBall.refreshBall();
				pl_score = 0;
				pr_score = 0;
				gameFinish=false;
			}
		}
		else if(pr_score>=scoreWin)
		{
			gameFinish=true;
			cv::putText(output,"Player 2 Win !!",cv::Point(screenX/2 + 20,screenY/3),2,2,cv::Scalar(240,0,240),3);
			cv::putText(output,"<--- Press Goal To Start Game --->",cv::Point(screenX/2 - 320,screenY/2 + 60),2,1,cv::Scalar(0,240,55),2);
			MyBall.refreshBall();
			if(dglp1 <= 5000 || dgrp2 <= 5000)
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
		//cv::imshow("normalize",output2);

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
