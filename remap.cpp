#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

// ��ӳ��ͼ�񣬴���������Ч��
void wave(const Mat &image, Mat &result) {
	// ӳ�����
	Mat srcX(image.rows, image.cols, CV_32F); // x-map
	Mat srcY(image.rows, image.cols, CV_32F); // y-map

	// ����ӳ�����
	for (int i = 0; i<image.rows; i++) {
		for (int j = 0; j<image.cols; j++) {
			// (i,j)���ص���λ��
			srcX.at<float>(i, j) = j;  // ������ͬһ��
			srcY.at<float>(i, j) = i + 3 * sin(j / 6.0);  // ԭ���ڵ�i�е����أ����ڸ���һ�����������ƶ�

			// ˮƽ��ת
			//srcX.at<float>(i,j)= image.cols-j-1;
			//srcY.at<float>(i,j)= i;
		}
	}

	// Ӧ��ӳ�����
	remap(image,		  // Դͼ��
		  result,		  // Ŀ��ͼ��
		  srcX,			  // xӳ��
		  srcY,			  // yӳ��
		  INTER_LINEAR);  // �����
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