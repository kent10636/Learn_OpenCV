#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "colorDetector.h"

using namespace std;

int main()
{
	// ---------- �������滻��ɫ ----------
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

	// ---------- floodFill���� ----------
	// �ҳ���ָ����ɫ�ӽ�����ɫ����������
	cv::floodFill(image,            // ����/���ͼ��
		cv::Point(100, 50),         // ��ʼ��
		cv::Scalar(255, 255, 255),  // �����ɫ
		(cv::Rect*)0,               // �������ı߽����
		cv::Scalar(35, 35, 35),     // ƫ�����С/�����ֵ
		cv::Scalar(35, 35, 35),     // ������ֵ��������ֵͨ�����
		cv::FLOODFILL_FIXED_RANGE); // ����ʼ�����رȽ�

	cv::namedWindow("floodFill Result");
	result = cdetect.process(image);
	cv::imshow("floodFill Result", image);

	cv::waitKey();
	return 0;
}