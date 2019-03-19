#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "colorDetector.h"

using namespace std;

int main()
{
	// 1. 创建图像处理器对象
	ColorDetector cdetect;

	// 2. 读取输入的图像
	cv::Mat image = cv::imread("boldt.jpg");
	if (image.empty())return 0;
	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);

	// 3. 设置输入参数
	cdetect.setTargetColor(230, 190, 130); // 这里表示蓝天

	// 4. 处理图像并显示结果
	cv::namedWindow("Result");
	cv::Mat result = cdetect.process(image);
	cv::imshow("Result", result);

	cv::waitKey();
	return 0;
}