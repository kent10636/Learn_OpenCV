#include <iostream>

#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>

using namespace std;

int main()
{
	// ---------- 分割并提取物体 ----------
	cv::Mat image = cv::imread("boldt.jpg");
	if (!image.data)return 0;
	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);

	cv::Rect rectangle(50, 25, 350, 280);  // 定义一个带边框的矩形，矩形外部的像素将会被标记为背景
	cv::Mat bgModel, fgModel;  // 模型（内部使用）
	cv::Mat result;  // 分割结果（4种可能的值）

	// grabCut分割算法
	cv::grabCut(image,           // 输入图像
		result,                  // 分割结果
		rectangle,               // 包含前景的矩形 
		bgModel, fgModel,        // 模型
		5,                       // 迭代次数
		cv::GC_INIT_WITH_RECT);  // 使用矩形

	cv::compare(result, cv::GC_PR_FGD, result, cv::CMP_EQ);  // 取得标记为“可能属于前景”的像素
	cv::Mat foreground(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));  // 创建一个白色图像
	image.copyTo(foreground, result);  // 不复制背景像素

	// 在原图像上绘制矩形
	cv::rectangle(image, rectangle, cv::Scalar(255, 255, 255), 1);
	
	cv::namedWindow("Image With Rectangle");
	cv::imshow("Image With Rectangle", image);

	// 显示结果
	cv::namedWindow("Foreground Object");
	cv::imshow("Foreground Object", foreground);

	cv::waitKey();
	return 0;
}