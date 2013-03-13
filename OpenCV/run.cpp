#include "chp1.h"
#include "chp3.h"
#include "chp4.h"
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
	/*
	auto img1 = cv::imread("t2.jpg");
	auto cd = ColorDetector();
	cd.setDistThreshold(200);
	auto result = cd.process(img1);
	showImg(result,"result");
	*/
	auto cdc = ColorDetectorController::getInstance();
	cdc->setImage("test.jpg");
	cdc->process();
	showImg(cdc->getResult(),"result");
	cv::waitKey(0);
}

void test_chp4()
{
	// 显示一个图像的颜色直方图 （单通道）
	auto img = cv::imread("lena.tif");
	showImg(img,"img");
	Histogram1D hgen;
	showImg(hgen.getHistImage(img),"hist");
	cv::Mat threshold;
	cv::threshold(img,threshold,130,255,cv::THRESH_BINARY);
	showImg(threshold,"threshold");
	// 图片反色
	showImg(hgen.inverse(img),"inverse");
	// stretch 增强对比度
	showImg(hgen.stretch(img),"stretch");
	showImg(hgen.stretch(img,20),"stretch 20");
	// equalize histogram
	auto r = hgen.equalize(img);
	showImg(hgen.equalize(img),"equalize");
	cv::waitKey(0);
}

void test_chp4_2()
{
	auto img = cv::imread("t3.jpg");
	showImg(img,"img");
	Histogram hc;
	img = hc.colorReduce(img); // 先reduce color
	showImg(img,"color reduce");
	cv::Mat ROI = img(cv::Rect(0,150,50,50));// 
	auto hist = hc.getHistogram(ROI);
	ContentDetector finder;
	finder.setHistogram(hist);
	finder.setThreshold(0.05f);
	auto result = finder.find(img);
	showImg(result,"result");
	cv::waitKey(0);
}

void test_chp4_3()
{
	auto img = cv::imread("t3.jpg");
	auto ROI = img(cv::Rect(0,0,50,50));
	int minSat = 60;
	Histogram  hc;
	auto hist = hc.getHueHistogram(ROI,minSat);
	ContentDetector finder;
	finder.setHistogram(hist);

}
int main()
{
	//run_chp1();
	//test_sharpen();
	//test_combine();
	//test_image_operators();
	test_chp4_2();
	return 0;
}