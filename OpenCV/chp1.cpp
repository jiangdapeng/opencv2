#include "chp1.h"
#include <functional>
using namespace std;

/**
* 对图像数据进行简单的操作，主要是访问图像的每一个像素的数值
*/
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
	int shift = static_cast<int>(
		log(static_cast<double>(div))/log(2.0));
	int MASK = 0xFF << shift;
	for(int i=0;i<h;++i)
	{
		// for each row
		// pay attention to method "ptr"
		uchar* data = img.ptr<uchar>(i);
		for( int j=0;j<nc;++j)
		{
			//data[j] = data[j]/div * div+half;
			data[j] = (data[j] & MASK)+half; //这个最快
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

void colorReduceIt(cv::Mat& img, std::function<int(int)> reduce)
{
	auto it = img.begin<cv::Vec3b>();
	auto end = img.end<cv::Vec3b>();
	while(it != end)
	{
		(*it)[0] = reduce((*it)[0]);
		(*it)[1] = reduce((*it)[1]);
		(*it)[2] = reduce((*it)[2]);
		++it;
	}
}

void showImg(const cv::Mat& img, const std::string& wname)
{
	cv::namedWindow(wname);
	cv::imshow(wname,img);
}

void sharpen(const cv::Mat& img, cv::Mat& result)
{
	result.create(img.size(),img.type());
	int nr = img.rows;
	int nc = img.cols * img.channels();
	for(int j=1;j<nr -1;++j)
	{
		// for rows from 1 to nr-1
		const uchar* above = img.ptr<uchar>(j-1);
		const uchar* current = img.ptr<uchar>(j);
		const uchar* below = img.ptr<uchar>(j+1);

		uchar* rc = result.ptr<uchar>(j);
		for(int i=1;i<nc-1;++i)
		{
			// for each bytes from 1 to nc-1
			rc[i] = cv::saturate_cast<uchar>(5 * current[i] - above[i] - below[i] - current[i-1] - current[i+1]);
		}
	}
	result.row(0).setTo(cv::Scalar(0));
	result.row(nr-1).setTo(cv::Scalar(0));
	result.col(0).setTo(cv::Scalar(0));
	result.col(img.cols-1).setTo(cv::Scalar(0));
}

void sharpen2(const cv::Mat& img, cv::Mat& result)
{
	result.create(img.size(),img.type());
	int nr = img.rows;
	int nc = img.cols;
	for(int j=1;j<nr -1;++j)
	{
		// for rows from 1 to nr-1
		auto above = img.ptr<cv::Vec3b>(j-1);
		auto current = img.ptr<cv::Vec3b>(j);
		auto below = img.ptr<cv::Vec3b>(j+1);

		auto rc = result.ptr<cv::Vec3b>(j);
		for(int i=1;i<nc-1;++i)
		{
			// for each bytes from 1 to nc-1
			for(int k=0;k<3;++k)
			{
				rc[i][k] = cv::saturate_cast<uchar>(5 * current[i][k] - above[i][k] - below[i][k] - current[i-1][k] - current[i+1][k]);
			}
		}
	}
	result.row(0).setTo(cv::Scalar(0));
	result.row(nr-1).setTo(cv::Scalar(0));
	result.col(0).setTo(cv::Scalar(0));
	result.col(img.cols-1).setTo(cv::Scalar(0));
}

void sharpen2D(const cv::Mat& img, cv::Mat& result)
{
	cv::Mat kernel(3,3,CV_32F,cv::Scalar(0));
	kernel.at<float>(1,1) = 5;
	kernel.at<float>(0,1) = -1;
	kernel.at<float>(1,0) = -1;
	kernel.at<float>(1,2) = -1;
	kernel.at<float>(2,1) = -1;
	cv::filter2D(img,result,img.depth(),kernel);
}