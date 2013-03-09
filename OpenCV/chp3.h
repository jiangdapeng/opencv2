#ifndef ASUWILL_OPENCV2_CHP3_H
#define ASUWILL_OPENCV2_CHP3_H

#include "opencv2.h"

// �ҳ�ͼ���и�����ɫ�����ص㣨��ɫ��ֵ��һ����Χ�ڣ�
class ColorDetector
{
public:
	ColorDetector();
	~ColorDetector();

	void setDistThreshold(int dt);
	int getDistThreshold() const;
	void setTarget(cv::Vec3b color);
	cv::Vec3b getTarget() const;

	// ���صõ��Ľ��
	cv::Mat process(const cv::Mat &img);

private:
	inline int getDist(cv::Vec3b color); 

private:
	int distThreshold; // ʶ��ʱ����ɫ��ֵ������
	cv::Vec3b targetColor;
	cv::Mat result;
};

#endif