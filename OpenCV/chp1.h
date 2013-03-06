#ifndef OPENCV2_CHP2_H
#define OPENCV2_CHP2_H

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <functional>

// show image in a window
void showImg(cv::Mat& img, const std::string& wname);

// add some salt to image
void salt(cv::Mat & img,int n);

// reduce image color by div
void colorReduce(cv::Mat& img, int div=64);

// reduce image color using self defined reduce method
void colorReduce2(cv::Mat& img, std::function<int(int)> reduce);

#endif