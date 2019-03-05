#include <iostream>
#include <random>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

void colorReduce(cv::Mat image, int div = 64) {
	int nl = image.rows;  // 行数
	int nc = image.cols * image.channels();  // 每行的元素数量

	for (int j = 0; j < nl; j++) {
		uchar* data = image.ptr<uchar>(j);  // 取得行j的地址
		for (int i = 0; i < nc; i++) {
			// 处理每个像素 ------------------------
			data[i] = data[i] / div * div + div / 2;
			// 像素处理结束 ------------------------
		}  // 一行结束
	}
}

int main() {
	Mat image = imread("boldt.jpg");

	colorReduce(image, 64);

	namedWindow("Image");
	imshow("Image", image);
	waitKey(0);

	return true;
}