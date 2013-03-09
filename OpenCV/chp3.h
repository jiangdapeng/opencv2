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
	void setTarget(const uchar red,const uchar green, const uchar blue);
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

// ����ģʽ��Controller
class ColorDetectorController
{
private:
	static ColorDetectorController* cdc;// singleton
	ColorDetector *cd;
	cv::Mat img;	// input image
	cv::Mat result; // process result
	ColorDetectorController() // private construction function
	{
		cd = new ColorDetector();
	}
public:
	static ColorDetectorController* getInstance()
	{
		if(cdc == NULL)
		{
			cdc = new ColorDetectorController();
		}
		return cdc;
	}
	~ColorDetectorController()
	{
		if(NULL != cd)
		{
			delete cd;
			cd = NULL;
		}
	}
	static void destroy()
	{
		if(NULL != cdc)
		{
			delete cdc;
		}
	}
	void setDistThreshold(int dist)
	{
		cd->setDistThreshold(dist);
	}
	int getDistThreshold() const
	{
		return cd->getDistThreshold();
	}

	void setTargetColor(const uchar red,const uchar green,const uchar blue)
	{
		cd->setTarget(red,green,blue);
	}
	void getTargetColor(uchar &red, uchar &green, uchar & blue) const
	{
		cv::Vec3b color = cd->getTarget();
		red = color[0];
		green = color[1];
		blue = color[2];
	}
	bool setImage(const std::string &filename)
	{
		img = cv::imread(filename);
		if(!img.data)
			return false;
		else
			return true;
	}
	cv::Mat getImage() const
	{
		return img;
	}

	void process()
	{
		result = cd->process(img);
	}

	cv::Mat getResult() const
	{
		return result;
	}
};



#endif