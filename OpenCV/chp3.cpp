#include "chp3.h"

using namespace std;

ColorDetector::ColorDetector():distThreshold(200)
{
	targetColor[0] = targetColor[1] = targetColor[2] = 0;
}

ColorDetector::~ColorDetector()
{
}

void ColorDetector::setDistThreshold(int dt)
{
	distThreshold = dt;
}

int ColorDetector::getDistThreshold() const
{
	return distThreshold;
}

void ColorDetector::setTarget(cv::Vec3b color)
{
	targetColor = color;
}

cv::Vec3b ColorDetector::getTarget() const
{
	return targetColor;
}

int ColorDetector::getDist(cv::Vec3b color)
{
	return abs(targetColor[0]-color[0])
		+ abs(targetColor[1] - color[1])
		+ abs(targetColor[2] - color[2]);
}

cv::Mat ColorDetector::process(const cv::Mat &img)
{
	result.create(img.rows,img.cols,CV_8U);
	auto it = img.begin<cv::Vec3b>();
	auto end = img.end<cv::Vec3b>();
	auto out = result.begin<uchar>();
	while(it != end)
	{
		if(getDist(*it) < distThreshold)
		{
			*out = 0;
		}
		else {
			*out = 255;
		}
		++it;
		++out;
	}
	return result;
}