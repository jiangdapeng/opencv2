#ifndef ASUWILL_OPENCV2_CHP3_H
#define ASUWILL_OPENCV2_CHP3_H

#include "opencv2.h"

// 找出图像中给定颜色的像素点（颜色差值在一定范围内）
class ColorDetector
{
public:
	ColorDetector();
	~ColorDetector();

	void setDistThreshold(int dt);
	int getDistThreshold() const;
	void setTarget(cv::Vec3b color);
	cv::Vec3b getTarget() const;

	// 返回得到的结果
	cv::Mat process(const cv::Mat &img);

private:
	inline int getDist(cv::Vec3b color); 

private:
	int distThreshold; // 识别时，颜色差值的上限
	cv::Vec3b targetColor;
	cv::Mat result;
};

#endif