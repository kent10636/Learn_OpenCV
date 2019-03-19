#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "colorDetector.h"

using namespace std;

int main()
{
	// ---------- 搜索并替换颜色 ----------
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

	// ---------- floodFill函数 ----------
	// 找出与指定颜色接近的颜色（连续区域）
	cv::floodFill(image,            // 输入/输出图像
		cv::Point(100, 50),         // 起始点
		cv::Scalar(255, 255, 255),  // 填充颜色
		(cv::Rect*)0,               // 填充区域的边界矩形
		cv::Scalar(35, 35, 35),     // 偏差的最小/最大阈值
		cv::Scalar(35, 35, 35),     // 正差阈值，两个阈值通常相等
		cv::FLOODFILL_FIXED_RANGE); // 与起始点像素比较

	cv::namedWindow("floodFill Result");
	result = cdetect.process(image);
	cv::imshow("floodFill Result", image);

	cv::waitKey();
	return 0;
}