#ifndef OPENCV2_CHP2_H
#define OPENCV2_CHP2_H

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <functional>

// show image in a window
void showImg(const cv::Mat& img, const std::string& wname);

// add some salt to image
void salt(cv::Mat & img,int n);

// reduce image color by div
void colorReduce(cv::Mat& img, int div=64);

// reduce image color using self defined reduce method
void colorReduce2(cv::Mat& img, std::function<int(int)> reduce);

// to visit data elements of image using iterator
void colorReduceIt(cv::Mat& img, std::function<int(int)> reduce);

// 图片增强对比
void sharpen(const cv::Mat& img, cv::Mat& result);

void sharpen2(const cv::Mat& img, cv::Mat& result);

// using cv::filter2D
void sharpen2D(const cv::Mat& img, cv::Mat& result);

#endif