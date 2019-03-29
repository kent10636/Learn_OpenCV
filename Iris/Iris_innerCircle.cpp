#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/types_c.h>

using namespace std;

int main() {
	string imageFile = "./iris/S6753S07.jpg";  // ͼ���ļ�·��

	cv::Mat srcImage = cv::imread(imageFile, -1);
	if (!srcImage.data)return 0;
	cv::namedWindow("Source Image");
	cv::imshow("Source Image", srcImage);

	cv::Mat image;  // ��Ŵ�������е�ͼ��

	int srcChannels = srcImage.channels();
	//cout << "Channels: " << srcChannels << endl;
	if (srcChannels != 1) {  // ��ԭͼ���ǻҶ�ͼ������ҶȻ�
		cv::Mat grayImage;
		cv::cvtColor(srcImage, grayImage, CV_RGB2GRAY);
		image = grayImage;
		cv::namedWindow("Gray Image");
		cv::imshow("Gray Image", grayImage);
	}
	else {
		image = srcImage;
	}

	/*
	//��ԭͼ��ҶȻ�
	if (srcImage.type() == CV_8UC1) {  // �Ҷ�ͼ
		image = srcImage;
	}
	else if (image.type() == CV_8UC3) {  // ��ɫͼ
		cv::Mat grayImage;
		cv::cvtColor(srcImage, grayImage, CV_RGB2GRAY);
		image = grayImage;
		cv::namedWindow("Gray Image");
		cv::imshow("Gray Image", grayImage);
	}
	*/

	// --------------------------------------------------
	// ���ͼ���������Ա����cv::HoughCircles�����е���ֵ
	cv::Mat contours;
	cv::Canny(image,  // �Ҷ�ͼ��
		contours,     // �������
		70,           // ����ֵ
		140);         // ����ֵ

	// ��ʾͼ���Ե����
	cv::namedWindow("Canny Contours");
	cv::imshow("Canny Contours", 255 - contours);

	// --------------------------------------------------
	// ��Ĥ��Բ���
	cv::GaussianBlur(image, image, cv::Size(5, 5), 1.5);  // ��˹ģ��/��˹ƽ��������ͼ������������ϸ�ڲ��
	//cv::namedWindow("Blured Image");
	//cv::imshow("Blured Image", image);

	std::vector<cv::Vec3f> circles;  // ���ڴ�ż����
	
	// ��һ��Ӧ��cv::HoughCircles����ʱʹ�õĲ���
	int canny_threshold = 140, 
		vote = 100, 
		min_rad = 10, max_rad = 80;
	// ��һ��Ӧ��cv::HoughCircles����
	cv::HoughCircles(image, circles, cv::HOUGH_GRADIENT,
		2,                  // �ۼ����ֱ��� (ͼ��ߴ� / 2) 
		20,                 // ����Բ֮�����С����
		canny_threshold,    // Canny���ӵĸ���ֵ����ֵ�߿ɲ�������ȷ�ı�Ե
		vote,               // ����ͶƱ����ͶƱ���߿ɲ�����׼ȷ��Բ
		min_rad, max_rad);  // ��С�����뾶
	int circles_num = circles.size();
	std::cout << "Circles: " << circles.size() << std::endl;

	if (circles_num > 1) {  // ��һ��ʱ��⵽2�������ϵ�Բ
		for (; circles_num > 1; vote += 10) {
			cv::HoughCircles(image, circles, cv::HOUGH_GRADIENT,
				2,
				20,
				canny_threshold,
				vote,
				min_rad, max_rad);
			circles_num = circles.size();
			std::cout << "Circles: " << circles_num << std::endl;
		}
	}
	else if (circles_num == 0) {  // ��һ��ʱ��ⲻ��Բ
		for (; circles_num == 0; vote -= 10) {
			cv::HoughCircles(image, circles, cv::HOUGH_GRADIENT,
				2,
				20,
				canny_threshold,
				vote,
				min_rad, max_rad);
			circles_num = circles.size();
			std::cout << "Circles: " << circles_num << std::endl;
		}
	}

	// ��Բ
	image = cv::imread(imageFile, -1);
	std::vector<cv::Vec3f>::const_iterator itc = circles.begin();
	while (itc != circles.end()) {
		cv::circle(image,
			cv::Point((*itc)[0], (*itc)[1]),  // Բ��
			(*itc)[2],                        // �뾶
			cv::Scalar(255),                  // ��ɫ 
			2);                               // ���

		++itc;
	}
	cv::namedWindow("Detected Circles");
	cv::imshow("Detected Circles", image);

	cv::waitKey();
	return 0;
}