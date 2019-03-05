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
	result = 0.7*image1 + 0.9*image2;  // ʹ�����ز�����
	namedWindow("Result with operators");
	imshow("Result with operators", result);
	//waitKey(0);

	// -------------------------------------------------
	image2 = cv::imread("rain.jpg", 0);
	vector<Mat> planes;  // ����3��ͼ�������
	split(image1, planes);  // ��1����ͨ��ͼ��ָ�Ϊ3����ͨ��ͼ��
	planes[1] += image2;  // �ӵ���ɫ(planes[0])/��ɫ(planes[1])/��ɫ(planes[2])ͨ����
	merge(planes, result);  // ��3����ͨ��ͼ��ϲ�Ϊ1����ͨ��ͼ��
	namedWindow("Result on green channel");
	imshow("Result on green channel", result);
	waitKey(0);

	return 0;
}