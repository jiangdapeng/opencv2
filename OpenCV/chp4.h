#ifndef ASUWILL_OPENCV2_CHP4_H
#define ASUWILL_OPENCV2_CHP4_H

#include "opencv2.h"

// histogram for gray image
class Histogram1D
{
public:
	Histogram1D()
	{
		histSize[0] = 256;
		hranges[0] = 0;
		hranges[1] = 255;
		ranges[0] = hranges;
		channels[0] = 1;
	}
	cv::MatND getHistogram(const cv::Mat& img)
	{
		cv::MatND hist;
		cv::calcHist(
			&img,
			1,         // from 1 image
			channels,
			cv::Mat(), // mask, here no mask is used
			hist,      // output
			1,		   // 1D histogram
			histSize,  //
			ranges);
		return hist;
	}

	cv::Mat getHistImage(const cv::Mat &img)
	{
		// compute histogram
		cv::MatND hist = getHistogram(img);

		// get max and min bin values
		double maxVal = 0;
		double minVal = 0;
		cv::minMaxLoc(hist,&minVal,&maxVal, 0, 0);

		cv::Mat histImg(histSize[0],histSize[0],CV_8U,cv::Scalar(255));

		// hpt = Highest Point
		int hpt = static_cast<int>(0.9*histSize[0]);
		// draw a vertical line for each bin
		for(int h = 0;h<histSize[0]; ++h)
		{
			float binVal = hist.at<float>(h);
			int intensity = static_cast<int>(binVal * hpt / maxVal);
			cv::line(
				histImg,
				cv::Point(h,histSize[0]),
				cv::Point(h,histSize[0]-intensity),
				cv::Scalar::all(0));
		}
		return histImg;
	}

	// using look-up table to modify image's apperance
	cv::Mat applyLookUp(const cv::Mat &img, const cv::Mat &lookup)
	{
		cv::Mat result;
		cv::LUT(img,lookup,result);
		return result;
	}

	// inverse color using applyLookUp
	cv::Mat inverse(const cv::Mat &img)
	{
		int dim = 256;
		cv::Mat lut(1,// 1 dimension
			&dim,     // 256 entries
			CV_8U);   // uchar
		for(int i=0;i<256;++i)
		{
			lut.at<uchar>(i) = 255-i;
		}
		return applyLookUp(img,lut);
	}
	// stretch （增强对比度）
	cv::Mat stretch(const cv::Mat &img, int minValue=0)
	{
		cv::MatND hist = getHistogram(img);
		int imin = 0;
		int imax = histSize[0]-1;
		for(;imin < histSize[0]; ++imin)
		{
			if(hist.at<float>(imin) > minValue)
				break;
		}
		for(;imax >=0; --imax)
		{
			if(hist.at<float>(imax) > minValue)
				break;
		}

		// construct look up table
		int dim = 256;
		cv::Mat lut(1,
			&dim,
			CV_8U);
		int seg = imax - imin;
		for(int i=0;i<256;++i)
		{
			if(i<imin)
				lut.at<uchar>(i) = 0;
			else if(i>imax)
				lut.at<uchar>(i) = 255;
			else
				lut.at<uchar>(i) = static_cast<uchar>(255*(i-imin)/seg + 0.5);
		}
		cv::Mat result;
		return applyLookUp(img,lut);
	}
	// equalize histogram
	cv::Mat equalize(const cv::Mat &img)
	{
		cv::Mat result;
		//cv::equalizeHist(img,result);// 这里运行会报错
		int all = img.rows * img.cols; // total number of pixels
		auto hist = getHistogram(img);
		float p[256];
		p[0] = hist.at<float>(0);
		for(int i=1;i<histSize[0];++i)
		{
			p[i] = hist.at<float>(i)+p[i-1];
		}
		for(int i=0;i<histSize[0];++i)
		{
			p[i] = p[i]/all;
		}
		// build lookuo table
		int dim = 256;
		cv::Mat lut(1,
			&dim,
			CV_8U);
		for(int i=0;i<256;++i)
		{
			lut.at<uchar>(i) = static_cast<uchar>(255*p[i]);// 这是核心
		}
		return applyLookUp(img,lut);
	}
private:
	int histSize[1]; // number of bins
	float hranges[2]; // range of pixel value
	const float* ranges[1];
	int channels[1]; // gray image
};


class Histogram
{
public:
	Histogram()
	{
		histSize[2] = histSize[1] = histSize[0] = 256;
		hranges[0] = 0;
		hranges[1] = 255;
		ranges[2] = ranges[1] = ranges[0] = hranges;
		channels[0] = 1;
		channels[1] = 2;
		channels[2] = 3;
	}
	cv::MatND getHistogram(const cv::Mat& img)
	{
		cv::MatND hist;
		cv::calcHist(
			&img,
			1,         // from 1 image
			channels,
			cv::Mat(), // mask, here no mask is used
			hist,      // output
			3,		   // 1D histogram
			histSize,  //
			ranges);
		return hist;
	}

	// get sparse histogram of color image
	cv::SparseMat getSparseHistogram(const cv::Mat &img)
	{
		cv::SparseMat hist(3,histSize,CV_32F);
		cv::calcHist(
			&img,
			1,         // from 1 image
			channels,
			cv::Mat(), // mask, here no mask is used
			hist,      // output
			3,		   // 1D histogram
			histSize,  //
			ranges);
		return hist;
	}
private:
	int histSize[3]; // number of bins
	float hranges[2]; // range of pixel value
	const float* ranges[3];
	int channels[3]; // color image
};

#endif