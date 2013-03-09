#include "chp1.h"
#include "chp3.h"
#include <iostream>
#include <sstream>
using namespace std;

void run_chp1()
{
	auto img = cv::imread("test.jpg");
	// whether readding file success
	if(!img.data)
	{
		cout<<"Reading image file failed"<<endl;
		return ;
	}
	cout<<"height:"<<img.size().height<<" width:"<<img.size().width<<endl;
	showImg(img,"img");
	cv::Mat oimg,vimg,img2,img3,img4;
	img2 = img; // 注意内部数据采用引用计数
	img.copyTo(img3); // 真的副本
	img.copyTo(img4);
	// 对称变换
	//// 水平
	//cv::flip(img,oimg,1);
	//cv::namedWindow("new horizontal");
	//cv::imshow("new horizontal",oimg);
	//// 垂直
	//cv::flip(img,vimg,0);
	//cv::namedWindow("new vertical");
	//cv::imshow("new vertical",vimg);

	//cv::flip(img,img,-1);
	//cv::namedWindow("img2");
	//cv::imshow("img2",img2);
	double duration = static_cast<double>(cv::getTickCount());
	colorReduce(img2,32);
	duration = static_cast<double>(cv::getTickCount()) - duration;
	cout<<"colorReduce(,32):"<<duration/cv::getTickFrequency()<<endl;

	showImg(img2,"img2");

	salt(img3,200);
	showImg(img3,"img3");

	duration = static_cast<double>(cv::getTickCount());
	colorReduce2(img3,[](int d){return d/32*32+16;});
	duration = static_cast<double>(cv::getTickCount()) - duration;
	cout<<"colorReduce2(,[]):"<<duration/cv::getTickFrequency()<<endl;

	showImg(img3,"new img3");

	duration = static_cast<double>(cv::getTickCount());
	colorReduceIt(img4,[](int d){return (d & (0xFF << 5))+16;});
	duration = static_cast<double>(cv::getTickCount()) - duration;
	cout<<"colorReduceIT(,&):"<<duration/cv::getTickFrequency()<<endl;

	showImg(img4,"img4");

	cv::waitKey(0);

	
}

void test_sharpen()
{
	auto img = cv::imread("test.jpg");
	showImg(img,"test.jpg");
	cv::Mat result;
	sharpen(img,result);
	showImg(result,"result");

	cv::Mat result2;
	sharpen2(img,result2);
	showImg(result2,"result2");

	cv::Mat result3;
	sharpen2D(img,result3);
	showImg(result3,"result3");
	cv::waitKey(0);
}

void test_combine()
{
	auto img1 = cv::imread("t3.jpg");
	auto img2 = cv::imread("t4.jpg");
	cv::Mat result;
	string title="title:";
	for(int i=0;i<10;i+=2)
	{
		for(int j=10;j>0;j-=2)
		{
			cout<< i << ":" << j<<endl;
			cv::addWeighted(img1,i*0.1,img2,j*0.1,0.5,result);
			ostringstream ss;
			ss << title << i << "+" << j;
			showImg(result,ss.str());
		}
	}
	cv::waitKey(0);
}

void test_image_operators()
{
	auto img1 = cv::imread("t3.jpg");
	auto img2 = cv::imread("t4.jpg");
	auto r1 = img1+img2;
	showImg(r1,"r1");

	auto r2 = img1 & img2;
	auto r3 = img1 | img2;
	showImg(r2,"&");
	showImg(r3,"|");
	int mask = 0xFF << 6;
	int half = 32;
	auto r4 = (img1 & cv::Scalar(mask,mask,mask))+cv::Scalar(half,half,half);
	showImg(r4,"r4");
	cv::waitKey(0);
}

void test_chp3()
{
	auto img1 = cv::imread("t2.jpg");
	auto cd = ColorDetector();
	cd.setDistThreshold(300);
	auto result = cd.process(img1);
	showImg(result,"result");
	cv::waitKey(0);
}
int main()
{
	//run_chp1();
	//test_sharpen();
	//test_combine();
	//test_image_operators();
	test_chp3();
	return 0;
}