#include "chp1.h"
#include <functional>
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

void colorReduce(cv::Mat& img, int div)
{
	int w = img.cols;
	int h = img.rows;
	int nc = w*img.channels();
	int half = div/2;
	for(int i=0;i<h;++i)
	{
		// for each row
		// pay attention to method "ptr"
		uchar* data = img.ptr<uchar>(i);
		for( int j=0;j<nc;++j)
		{
			data[j] = data[j]/div * div+half;
		}
	}
}

void colorReduce2(cv::Mat& img, std::function<int(int)> reduce)
{
	int w = img.cols;
	int h = img.rows;
	int nc = w*img.channels();
	for(int i=0;i<h;++i)
	{
		// for each row
		// pay attention to method "ptr"
		uchar* data = img.ptr<uchar>(i);
		for( int j=0;j<nc;++j)
		{
			data[j] = reduce(data[j]);
		}
	}
}

void showImg(cv::Mat& img, const std::string& wname)
{
	cv::namedWindow(wname);
	cv::imshow(wname,img);
}

