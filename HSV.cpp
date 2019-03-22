// HSV(Hueɫ����Saturation���Ͷȡ�Value����)

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2\imgproc\types_c.h>

#include <iostream>
#include <vector>

void detectHScolor(const cv::Mat& image,  // ����ͼ��input image 
	double minHue, double maxHue,	      // ɫ������
	double minSat, double maxSat,	      // ���Ͷ�����
	cv::Mat& mask) {				      // �������
	// ת����HSV�ռ�
	cv::Mat hsv;
	cv::cvtColor(image, hsv, CV_BGR2HSV);

	// ��3��ͨ���ָ��3��ͼ����
	std::vector<cv::Mat> channels;
	cv::split(hsv, channels);
	// channels[0]��ɫ��
	// channels[1]�Ǳ��Ͷ�
	// channels[2]������

	cv::Mat mask1;
	cv::Mat mask2;
	
	// ɫ������
	cv::threshold(channels[0], mask1, maxHue, 255, cv::THRESH_BINARY_INV);  // С��maxHue
	cv::threshold(channels[0], mask2, minHue, 255, cv::THRESH_BINARY);  // ����minHue
	cv::Mat hueMask;  // ɫ������
	if (minHue < maxHue)hueMask = mask1 & mask2;
	else hueMask = mask1 | mask2;  // ������䴩Խ0��������

	// ���Ͷ�����
	cv::threshold(channels[1], mask1, maxSat, 255, cv::THRESH_BINARY_INV);  // С��maxSat
	cv::threshold(channels[1], mask2, minSat, 255, cv::THRESH_BINARY);  // ����minSat
	cv::Mat satMask;  // ���Ͷ�����
	satMask = mask1 & mask2;

	// �������
	mask = hueMask&satMask;
}

int main()
{
	cv::Mat image = cv::imread("boldt.jpg");
	if (!image.data)return 0;
	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);

	// ת����HSVɫ�ʿռ�
	cv::Mat hsv;
	cv::cvtColor(image, hsv, CV_BGR2HSV);

	// ��3��ͨ���ָ��3��ͼ����
	std::vector<cv::Mat> channels;
	cv::split(hsv, channels);
	// channels[0]��ɫ������ʾ��ɫ����ɫ���ƣ����ɫ����ɫ����Ӧ��ͬ��ɫ��ֵ
	// channels[1]�Ǳ��Ͷȣ���ɫ�����޳̶�
	// channels[2]�����ȣ���ɫ�Ĺ����̶�

	// ��ʾɫ��ͼ
	cv::namedWindow("Hue");
	cv::imshow("Hue", channels[0]);
	// ��ʾ���Ͷ�ͼ
	cv::namedWindow("Saturation");
	cv::imshow("Saturation", channels[1]);
	// ��ʾ����ͼ
	cv::namedWindow("Value");
	cv::imshow("Value", channels[2]);

	// --------------------------------------------------
	// �̶����ȵ�ͼƬ
	cv::Mat newImage;
	cv::Mat tmp(channels[2].clone());
	channels[2] = 255;  // �������ص�����ͨ��ֵ��Ϊ255
	cv::merge(channels, hsv);  // ���ºϲ�ͨ��
	cv::cvtColor(hsv, newImage, CV_HSV2BGR);  // ת����BGR
	cv::namedWindow("Fixed Value Image");
	cv::imshow("Fixed Value Image", newImage);

	// �̶����Ͷȵ�ͼƬ
	channels[1] = 255;
	channels[2] = tmp;
	cv::merge(channels, hsv);
	cv::cvtColor(hsv, newImage, CV_HSV2BGR);
	cv::namedWindow("Fixed Saturation Image");
	cv::imshow("Fixed Saturation Image", newImage);

	// �̶����Ⱥ͹̶����Ͷȵ�ͼƬ
	channels[1] = 255;
	channels[2] = 255;
	cv::merge(channels, hsv);
	cv::cvtColor(hsv, newImage, CV_HSV2BGR);
	cv::namedWindow("Fixed Saturation/Value Image");
	cv::imshow("Fixed Saturation/Value Image", newImage);

	// --------------------------------------------------
	// ��Ϊ����ͼ����չʾ���п��ܵ�ɫ��/���Ͷ����
	cv::Mat hs(128, 360, CV_8UC3);
	for (int h = 0; h < 360; h++) {
		for (int s = 0; s < 128; s++) {
			hs.at<cv::Vec3b>(s, h)[0] = h / 2;        // ����ɫ���Ƕ�
			hs.at<cv::Vec3b>(s, h)[1] = 255 - s * 2;  // ���ͶȴӸߵ���
			hs.at<cv::Vec3b>(s, h)[2] = 255;          // ����
		}
	}
	cv::cvtColor(hs, newImage, CV_HSV2BGR);
	cv::namedWindow("Hue/Saturation");
	cv::imshow("Hue/Saturation", newImage);

	// --------------------------------------------------
	// ���Է�ɫ����㷨
	image = cv::imread("girl.jpg");
	if (!image.data)return 0;
	cv::namedWindow("Original image");
	cv::imshow("Original image", image);

	// ����ɫ
	cv::Mat mask;
	detectHScolor(image,
		160, 10,  // ɫ��Ϊ320����20�� 
		25, 166,  // ���Ͷ�Ϊ~0.1��0.65
		mask);

	// ��ʾʹ��������ͼ��
	cv::Mat detected(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	image.copyTo(detected, mask);
	cv::imshow("Detection Result", detected);

	cv::waitKey();
	return 0;
}