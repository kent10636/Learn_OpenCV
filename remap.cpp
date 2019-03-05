#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

// 重映射图像，创建波浪形效果
void wave(const Mat &image, Mat &result) {
	// 映射参数
	Mat srcX(image.rows, image.cols, CV_32F); // x-map
	Mat srcY(image.rows, image.cols, CV_32F); // y-map

	// 创建映射参数
	for (int i = 0; i<image.rows; i++) {
		for (int j = 0; j<image.cols; j++) {
			// (i,j)像素的新位置
			srcX.at<float>(i, j) = j;  // 保持在同一列
			srcY.at<float>(i, j) = i + 3 * sin(j / 6.0);  // 原来在第i行的像素，现在根据一个正弦曲线移动

			// 水平翻转
			//srcX.at<float>(i,j)= image.cols-j-1;
			//srcY.at<float>(i,j)= i;
		}
	}

	// 应用映射参数
	remap(image,		  // 源图像
		  result,		  // 目标图像
		  srcX,			  // x映射
		  srcY,			  // y映射
		  INTER_LINEAR);  // 填补方法
}

int main() {
	Mat image = imread("boldt.jpg");
	if (!image.data)return 0;

	namedWindow("Image");
	imshow("Image", image);
	
	Mat result;

	wave(image, result);

	namedWindow("Remapped image");
	imshow("Remapped image", result);
	waitKey();

	return 0;
}