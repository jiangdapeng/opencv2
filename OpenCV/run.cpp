#include "chp1.h"
#include <iostream>
using namespace std;

int main()
{
	auto img = cv::imread("test.jpg");
	// whether readding file success
	if(!img.data)
	{
		cout<<"Reading image file failed"<<endl;
		return 1;
	}
	cout<<"height:"<<img.size().height<<" width:"<<img.size().width<<endl;
	showImg(img,"img");
	// 对称变换
	cv::Mat oimg,vimg,img2,img3;
	img2 = img;
	img.copyTo(img3);
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

	colorReduce(img2,32);
	showImg(img2,"img2");
	salt(img3,200);
	showImg(img3,"img3");

	colorReduce2(img3,[](int d){return d/32*32+16;});
	showImg(img3,"new img3");
	cv::waitKey(0);
	return 0;
}