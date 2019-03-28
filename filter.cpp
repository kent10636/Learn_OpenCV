#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

int main()
{
	cv::Mat image = cv::imread("boldt.jpg", 0);
	if (!image.data)return 0;
	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);

	// --------------------------------------------------
	// 用一个均值滤波器模糊图像
	cv::Mat result;
	cv::blur(image, result, cv::Size(5, 5));
	cv::namedWindow("Mean filtered Image");
	cv::imshow("Mean filtered Image", result);

	// --------------------------------------------------
	// 用一个9x9均值滤波器模糊图像
	cv::blur(image, result, cv::Size(9, 9));
	cv::namedWindow("Mean filtered Image (9x9)");
	cv::imshow("Mean filtered Image (9x9)", result);

	// --------------------------------------------------
	// 高斯函数模糊图像
	cv::GaussianBlur(image, result,
		cv::Size(5, 5),  // 滤波器尺寸
		1.5);			 // 控制高斯曲线形状的参数
	cv::namedWindow("Gaussian filtered Image");
	cv::imshow("Gaussian filtered Image", result);

	cv::GaussianBlur(image, result, cv::Size(9, 9), 1.7);
	cv::namedWindow("Gaussian filtered Image (9x9)");
	cv::imshow("Gaussian filtered Image (9x9)", result);

	// --------------------------------------------------
	// 计算高斯滤波器的归一化系数
	// 根据高斯函数的sigma值，获取高斯滤波器内核 (sigma = 1.5)
	cv::Mat gauss = cv::getGaussianKernel(9, 1.5, CV_32F);
	cv::Mat_<float>::const_iterator it = gauss.begin<float>();
	cv::Mat_<float>::const_iterator itend = gauss.end<float>();
	std::cout << "1.5 = [";
	for (; it != itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;

	// 根据高斯函数的sigma值，获取高斯滤波器内核 (sigma = 0.5)
	gauss = cv::getGaussianKernel(9, 0.5, CV_32F);
	it = gauss.begin<float>();
	itend = gauss.end<float>();
	std::cout << "0.5 = [";
	for (; it != itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;

	// 根据高斯函数的sigma值，获取高斯滤波器内核 (sigma = 2.5)
	gauss = cv::getGaussianKernel(9, 2.5, CV_32F);
	it = gauss.begin<float>();
	itend = gauss.end<float>();
	std::cout << "2.5 = [";
	for (; it != itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;

	// 获取高斯滤波器内核 (9 elements)
	gauss = cv::getGaussianKernel(9, -1, CV_32F);
	it = gauss.begin<float>();
	itend = gauss.end<float>();
	std::cout << "9 = [";
	for (; it != itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;

	// --------------------------------------------------
	// 缩减像素采样
	// 将图像尺寸缩小到1/4(错误方式，只保留每4行(列)像素中的1行(列)，高频成分未去除，锯齿明显，图像效果差)
	image = cv::imread("boldt.jpg", 0);
	cv::Mat reduced(image.rows / 4, image.cols / 4, CV_8U);
	for (int i = 0; i<reduced.rows; i++)
		for (int j = 0; j<reduced.cols; j++)
			reduced.at<uchar>(i, j) = image.at<uchar>(i * 4, j * 4);
	cv::resize(reduced, reduced, cv::Size(), 4, 4, cv::INTER_NEAREST);  //放大四倍显示
	cv::namedWindow("Badly Reduced Image");
	cv::imshow("Badly Reduced Image", reduced);

	// 首先去除高频成分(正确方式)
	cv::GaussianBlur(image, image, cv::Size(11, 11), 1.75);
	// 只保留每4个像素中的1个
	cv::Mat reduced2(image.rows / 4, image.cols / 4, CV_8U);
	for (int i = 0; i<reduced2.rows; i++)
		for (int j = 0; j<reduced2.cols; j++)
			reduced2.at<uchar>(i, j) = image.at<uchar>(i * 4, j * 4);
	cv::Mat newImage;
	
	cv::resize(reduced2, newImage, cv::Size(), 4, 4, cv::INTER_NEAREST);  //放大四倍显示(最邻近插值)
	cv::namedWindow("Reduced Image(Nearest Resizing)");
	cv::imshow("Reduced Image(Nearest Resizing)", newImage);

	cv::resize(reduced2, newImage, cv::Size(), 4, 4, cv::INTER_LINEAR);  //放大四倍显示(双线性插值)
	cv::namedWindow("Reduced Image(Bilinear Resizing)");
	cv::imshow("Reduced Image(Bilinear Resizing)", newImage);

	// --------------------------------------------------
	// 消除椒盐噪声
	image = cv::imread("salted.bmp", 0);
	if (!image.data)return 0;
	cv::namedWindow("S&P Image");
	cv::imshow("S&P Image", image);

	// 使用均值滤波器（无法消除噪声）
	cv::blur(image, result, cv::Size(5, 5));
	cv::namedWindow("Mean Filtered Image");
	cv::imshow("Mean Filtered Image", result);

	// 使用中值滤波器（成功消除噪声）
	cv::medianBlur(image, result, 5);  // 最后一个参数是滤波器尺寸
	cv::namedWindow("Median Filtered Image");
	cv::imshow("Median Filtered Image", result);

	cv::waitKey();
	return 0;
}