#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "colorDetector.h"

using namespace std;

int main()
{
	// 1. ����ͼ����������
	ColorDetector cdetect;

	// 2. ��ȡ�����ͼ��
	cv::Mat image = cv::imread("boldt.jpg");
	if (image.empty())return 0;
	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);

	// 3. �����������
	cdetect.setTargetColor(230, 190, 130); // �����ʾ����

	// 4. ����ͼ����ʾ���
	cv::namedWindow("Result");
	cv::Mat result = cdetect.process(image);
	cv::imshow("Result", result);

	cv::waitKey();
	return 0;
}