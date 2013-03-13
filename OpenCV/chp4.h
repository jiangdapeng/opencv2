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
		channels[0] = 0;
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

// color histogram
class Histogram
{
public:
	Histogram()
	{
		histSize[2] = histSize[1] = histSize[0] = 256;
		hranges[0] = 0;
		hranges[1] = 255;
		ranges[2] = ranges[1] = ranges[0] = hranges;
		channels[0] = 0;
		channels[1] = 1;
		channels[2] = 2;
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
			3,		   // histogram
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
			3,		   // 3D histogram
			histSize,  //
			ranges);
		return hist;
	}

	// 计算 1D Hue histogram with mask
	cv::MatND getHueHistogram(const cv::Mat &img, int minSat)
	{
		cv::MatND hist;
		cv::Mat hsv;
		cv::cvtColor(img,hsv,CV_BGR2HSV);// 颜色空间转换
		cv::Mat mask;
		if (minSat > 0)
		{
			// split the 3 channels into 3 images
			std::vector<cv::Mat> v;
			cv::split(hsv,v);
			// mask out low saturated pixels
			cv::threshold(v[1],mask,minSat,255,cv::THRESH_BINARY);
		}
		hranges[0] = 0;
		hranges[1] = 180; // attention!
		channels[0] = 0;  // the hue channel
		cv::calcHist(&hsv,
			1,   // 1 image
			channels,
			mask,
			hist,// output
			1,   // 1D
			histSize,
			ranges
			);
		return hist;
	}
	// reduce color
	cv::Mat colorReduce(const cv::Mat &img, int div=32)
	{
		int w = img.cols;
		int h = img.rows;
		int nc = w*img.channels();
		cv::Mat result;
		result.create(h,w,img.type());
		int half = div/2;
		int shift = static_cast<int>(
			log(static_cast<double>(div))/log(2.0));
		int MASK = 0xFF << shift;
		for(int i=0;i<h;++i)
		{
			// for each row
			// pay attention to method "ptr"
			// return a row
			const uchar* data = img.ptr<uchar>(i);
			uchar* out = result.ptr<uchar>(i);
			for( int j=0;j<nc;++j)
			{
				//data[j] = data[j]/div * div+half;
				out[j] = (data[j] & MASK)+half; //这个最快
			}
		}
		return result;
	}
private:
	int histSize[3]; // number of bins
	float hranges[2]; // range of pixel value
	const float* ranges[3];
	int channels[3]; // color image
};

// using histogram to detect
class ContentDetector
{
public:
	ContentDetector():threshold(-1.0f)
	{
		hranges[0] = 0.0;
		hranges[1] = 255.0;
		for(int i=0;i<3;++i)
			ranges[i] = hranges;
		channels[0] = 0;
		channels[1] = 1;
		channels[2] = 2;
	}
	void setThreshold(float t)
	{
		threshold = t;
	}
	float getThreshold() const
	{
		return threshold;
	}

	// histogram used to detect content
	void setHistogram(const cv::MatND &h)
	{
		hist = h;
		cv::normalize(hist,hist,1.0);
	}

	void setRanges(float min, float max)
	{
		hranges[0] = min;
		hranges[1] = max;
	}
	cv::Mat find(const cv::Mat &img)
	{
		cv::Mat result;
		cv::calcBackProject(&img,// source
			1,					// 1 image
			channels,		// image channels
			hist,				// histogram
			result,				// result
			ranges,				// ranges
			255.0);				// scale

		if(threshold > 0.0)
		{
			cv::threshold(result,result,255*threshold,255,cv::THRESH_BINARY);
		}
		return result;
	}

private:
	float hranges[2];
	const float* ranges[3];
	int channels[3];
	float threshold;
	cv::MatND hist;
};

#endif