// HSV(Hue色调、Saturation饱和度、Value亮度)

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2\imgproc\types_c.h>

#include <iostream>
#include <vector>

void detectHScolor(const cv::Mat& image,  // 输入图像input image 
	double minHue, double maxHue,	      // 色调区间
	double minSat, double maxSat,	      // 饱和度区间
	cv::Mat& mask) {				      // 输出掩码
	// 转换到HSV空间
	cv::Mat hsv;
	cv::cvtColor(image, hsv, CV_BGR2HSV);

	// 把3个通道分割进3幅图像中
	std::vector<cv::Mat> channels;
	cv::split(hsv, channels);
	// channels[0]是色调
	// channels[1]是饱和度
	// channels[2]是亮度

	cv::Mat mask1;
	cv::Mat mask2;
	
	// 色调掩码
	cv::threshold(channels[0], mask1, maxHue, 255, cv::THRESH_BINARY_INV);  // 小于maxHue
	cv::threshold(channels[0], mask2, minHue, 255, cv::THRESH_BINARY);  // 大于minHue
	cv::Mat hueMask;  // 色调掩码
	if (minHue < maxHue)hueMask = mask1 & mask2;
	else hueMask = mask1 | mask2;  // 如果区间穿越0度中轴线

	// 饱和度掩码
	cv::threshold(channels[1], mask1, maxSat, 255, cv::THRESH_BINARY_INV);  // 小于maxSat
	cv::threshold(channels[1], mask2, minSat, 255, cv::THRESH_BINARY);  // 大于minSat
	cv::Mat satMask;  // 饱和度掩码
	satMask = mask1 & mask2;

	// 组合掩码
	mask = hueMask&satMask;
}

int main()
{
	cv::Mat image = cv::imread("boldt.jpg");
	if (!image.data)return 0;
	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);

	// 转换成HSV色彩空间
	cv::Mat hsv;
	cv::cvtColor(image, hsv, CV_BGR2HSV);

	// 把3个通道分割进3幅图像中
	std::vector<cv::Mat> channels;
	cv::split(hsv, channels);
	// channels[0]是色调，表示主色，颜色名称（如红色、绿色）对应不同的色调值
	// channels[1]是饱和度，颜色的鲜艳程度
	// channels[2]是亮度，颜色的光亮程度

	// 显示色调图
	cv::namedWindow("Hue");
	cv::imshow("Hue", channels[0]);
	// 显示饱和度图
	cv::namedWindow("Saturation");
	cv::imshow("Saturation", channels[1]);
	// 显示亮度图
	cv::namedWindow("Value");
	cv::imshow("Value", channels[2]);

	// --------------------------------------------------
	// 固定亮度的图片
	cv::Mat newImage;
	cv::Mat tmp(channels[2].clone());
	channels[2] = 255;  // 所有像素的亮度通道值改为255
	cv::merge(channels, hsv);  // 重新合并通道
	cv::cvtColor(hsv, newImage, CV_HSV2BGR);  // 转换回BGR
	cv::namedWindow("Fixed Value Image");
	cv::imshow("Fixed Value Image", newImage);

	// 固定饱和度的图片
	channels[1] = 255;
	channels[2] = tmp;
	cv::merge(channels, hsv);
	cv::cvtColor(hsv, newImage, CV_HSV2BGR);
	cv::namedWindow("Fixed Saturation Image");
	cv::imshow("Fixed Saturation Image", newImage);

	// 固定亮度和固定饱和度的图片
	channels[1] = 255;
	channels[2] = 255;
	cv::merge(channels, hsv);
	cv::cvtColor(hsv, newImage, CV_HSV2BGR);
	cv::namedWindow("Fixed Saturation/Value Image");
	cv::imshow("Fixed Saturation/Value Image", newImage);

	// --------------------------------------------------
	// 人为生成图像来展示所有可能的色调/饱和度组合
	cv::Mat hs(128, 360, CV_8UC3);
	for (int h = 0; h < 360; h++) {
		for (int s = 0; s < 128; s++) {
			hs.at<cv::Vec3b>(s, h)[0] = h / 2;        // 所有色调角度
			hs.at<cv::Vec3b>(s, h)[1] = 255 - s * 2;  // 饱和度从高到低
			hs.at<cv::Vec3b>(s, h)[2] = 255;          // 常数
		}
	}
	cv::cvtColor(hs, newImage, CV_HSV2BGR);
	cv::namedWindow("Hue/Saturation");
	cv::imshow("Hue/Saturation", newImage);

	// --------------------------------------------------
	// 测试肤色检测算法
	image = cv::imread("girl.jpg");
	if (!image.data)return 0;
	cv::namedWindow("Original image");
	cv::imshow("Original image", image);

	// 检测肤色
	cv::Mat mask;
	detectHScolor(image,
		160, 10,  // 色调为320度至20度 
		25, 166,  // 饱和度为~0.1至0.65
		mask);

	// 显示使用掩码后的图像
	cv::Mat detected(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	image.copyTo(detected, mask);
	cv::imshow("Detection Result", detected);

	cv::waitKey();
	return 0;
}