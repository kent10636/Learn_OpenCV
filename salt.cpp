#include <iostream>
#include <random>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

void salt(Mat image, int n) {
	// C++11随机数生成器
	default_random_engine generator;
	uniform_int_distribution<int> randomRow(0, image.rows - 1);
	uniform_int_distribution<int> randomCol(0, image.cols - 1);

	int i, j;
	for (int k = 0; k < n; k++) {
		// 随机生成图形位置
		i = randomCol(generator);
		j = randomRow(generator);
		if (image.type() == CV_8UC1) {  // 灰度图像（把单个8位数值设置为255即可得到白色像素）
			// 单通道8位图像
			image.at<uchar>(j, i) = 255;
		}
		else if (image.type() == CV_8UC3) {  // 彩色图像（需把3个颜色通道都设置为255才能得到白色像素）
			// 3通道图像
			image.at<Vec3b>(j, i)[0] = 255;
			image.at<Vec3b>(j, i)[1] = 255;
			image.at<Vec3b>(j, i)[2] = 255;
		}
	}
}

int main() {
	Mat image = imread("boldt.jpg",1);
	
	salt(image, 3000);

	namedWindow("Image");
	imshow("Image", image);
	waitKey(0);

	return true;
}