#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
using namespace std;

void salt(cv::Mat & img,int n)
{
	// 给图像增加一些噪点
	int w = img.size().width;
	int h = img.size().height;
	for(int k=0;k<n;++k)
	{
		int i = rand()%w;
		int j = rand()%h;
		if(img.channels() == 1)
		{
			img.at<uchar>(j,i) = 255;
		}
		else if(img.channels() == 3)
		{
			img.at<cv::Vec3b>(j,i)[0] = 255;
			img.at<cv::Vec3b>(j,i)[1] = 255;
			img.at<cv::Vec3b>(j,i)[2] = 255;
		}
	}
}

void colorRecude(cv::Mat& img, int div=64)
{
	int w = img.cols;
	int h = img.rows;
	int nc = w*img.channels();
	int half = div/2;
	for(int i=0;i<h;++i)
	{
		// for each row
		uchar* data = img.ptr<uchar>(i);
		for( int j=0;j<nc;++j)
		{
			data[j] = data[j]/div * div+half;
		}
	}
}

void showImg(cv::Mat& img, const std::string& wname)
{
	cv::namedWindow(wname);
	cv::imshow(wname,img);
}

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

	colorRecude(img2,32);
	showImg(img2,"img2");
	salt(img3,200);
	showImg(img3,"img3");
	cv::waitKey(0);
	return 0;
}