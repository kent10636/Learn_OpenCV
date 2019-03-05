#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main() {
	Mat image1 = imread("boldt.jpg");
	Mat image2 = imread("rain.jpg");
	if (!image1.data || !image2.data)return 0;

	namedWindow("Image 1");
	imshow("Image 1", image1);
	namedWindow("Image 2");
	imshow("Image 2", image2);

	Mat result;

	addWeighted(image1, 0.7, image2, 0.9, 0., result);

	namedWindow("Result");
	imshow("Result", result);
	//waitKey(0);

	// -------------------------------------------------
	result = 0.7*image1 + 0.9*image2;  // 使用重载操作符
	namedWindow("Result with operators");
	imshow("Result with operators", result);
	//waitKey(0);

	// -------------------------------------------------
	image2 = cv::imread("rain.jpg", 0);
	vector<Mat> planes;  // 创建3幅图像的向量
	split(image1, planes);  // 将1个三通道图像分割为3个单通道图像
	planes[1] += image2;  // 加到蓝色(planes[0])/绿色(planes[1])/红色(planes[2])通道上
	merge(planes, result);  // 将3个单通道图像合并为1个三通道图像
	namedWindow("Result on green channel");
	imshow("Result on green channel", result);
	waitKey(0);

	return 0;
}