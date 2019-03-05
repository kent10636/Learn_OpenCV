#include <iostream>
#include <random>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

void colorReduce(cv::Mat image, int div = 64) {
	int nl = image.rows;  // ����
	int nc = image.cols * image.channels();  // ÿ�е�Ԫ������

	for (int j = 0; j < nl; j++) {
		uchar* data = image.ptr<uchar>(j);  // ȡ����j�ĵ�ַ
		for (int i = 0; i < nc; i++) {
			// ����ÿ������ ------------------------
			data[i] = data[i] / div * div + div / 2;
			// ���ش������ ------------------------
		}  // һ�н���
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