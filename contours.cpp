#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "linefinder.h"
#include "edgedetector.h"

#define PI 3.1415926

using namespace std;

int main()
{
	cv::Mat image = cv::imread("road.jpg", 0);
	if (!image.data)return 0;
	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);

	// ����Sobel����
	EdgeDetector ed;
	ed.computeSobel(image);

	// Display the Sobel orientation
	cv::namedWindow("Sobel (orientation)");
	cv::imshow("Sobel (orientation)", ed.getSobelOrientationImage());
	//cv::imwrite("ori.bmp", ed.getSobelOrientationImage());

	// Display the Sobel(����ֵ)
	cv::namedWindow("Sobel (low threshold)");
	cv::imshow("Sobel (low threshold)", ed.getBinaryMap(125));

	// Display the Sobel(����ֵ)
	cv::namedWindow("Sobel (high threshold)");
	cv::imshow("Sobel (high threshold)", ed.getBinaryMap(350));

	// --------------------------------------------------
	// Ӧ��Canny�㷨
	cv::Mat contours;
	cv::Canny(image,  // �Ҷ�ͼ��
		contours,     // �������
		125,          // ����ֵ
		350);         // ����ֵ

	// Display the image of contours
	cv::namedWindow("Canny Contours");
	cv::imshow("Canny Contours", 255 - contours);

	// --------------------------------------------------
	// �û���任(Hough tranform)���ֱ��
	std::vector<cv::Vec2f> lines;
	cv::HoughLines(contours, lines, 1, 
		PI / 180,  // ��������ʾ�������������п��ܵĽǶ�
		50);       // ��СͶƱ��

	// ����ֱ��
	cv::Mat result(contours.rows, contours.cols, CV_8U, cv::Scalar(255));
	image.copyTo(result);
	std::cout << "Lines detected: " << lines.size() << std::endl;

	std::vector<cv::Vec2f>::const_iterator it = lines.begin();
	while (it != lines.end()) {
		float rho = (*it)[0];    // ��һ��Ԫ���Ǿ���rho
		float theta = (*it)[1];  // �ڶ���Ԫ���ǽǶ�theta

		if (theta < PI / 4. || theta > 3.*PI / 4.) {  // ��ֱ�ߣ����£�
			cv::Point pt1(rho / cos(theta), 0);  // ֱ�����һ�еĽ����	
			cv::Point pt2((rho - result.rows*sin(theta)) / cos(theta), result.rows);  // ֱ�������һ�еĽ����
			cv::line(result, pt1, pt2, cv::Scalar(255), 1);  // ����ɫ����
		}
		else {  // ˮƽ�ߣ����£�			
			cv::Point pt1(0, rho / sin(theta));  // ֱ�����һ�еĽ����		
			cv::Point pt2(result.cols, (rho - result.cols*cos(theta)) / sin(theta));  // ֱ�������һ�еĽ����
			cv::line(result, pt1, pt2, cv::Scalar(255), 1);  // ����ɫ����
		}
		std::cout << "line: (" << rho << "," << theta << ")\n";
		++it;
	}

	// Display the detected line image
	cv::namedWindow("Lines with Hough");
	cv::imshow("Lines with Hough", result);

	// --------------------------------------------------
	// ����LineFinder���ʵ��
	LineFinder ld;

	// ���ø��ʻ���任�Ĳ���
	ld.setLineLengthAndGap(100, 20);
	ld.setMinVote(60);

	// ���ֱ�߲�����
	std::vector<cv::Vec4i> li = ld.findLines(contours);
	ld.drawDetectedLines(image);
	cv::namedWindow("Lines with HoughP");
	cv::imshow("Lines with HoughP", image);

	// --------------------------------------------------
	// ���Բ
	image = cv::imread("chariot.jpg", 0);

	cv::GaussianBlur(image, image, cv::Size(5, 5), 1.5);
	std::vector<cv::Vec3f> circles;  // ��ż����
	// cv::HoughCircles������Canny��������任��ϣ�����֮ǰҪ��ͼ�����ƽ�������Լ�������
	cv::HoughCircles(image, circles, cv::HOUGH_GRADIENT,
		2,        // �ۼ����ֱ��� (ͼ��ߴ� / 2) 
		20,       // ����Բ֮�����С����
		200,      // Canny���ӵĸ���ֵ
		60,       // ����ͶƱ��
		15, 50);  // ��С�����뾶

	std::cout << "Circles: " << circles.size() << std::endl;

	// ��Բ
	image = cv::imread("chariot.jpg", 0);
	std::vector<cv::Vec3f>::const_iterator itc = circles.begin();
	while (itc != circles.end()) {
		cv::circle(image,
			cv::Point((*itc)[0], (*itc)[1]), // Բ��
			(*itc)[2],                       // �뾶
			cv::Scalar(255),                 // ��ɫ 
			2);                              // ���

		++itc;
	}
	cv::namedWindow("Detected Circles");
	cv::imshow("Detected Circles", image);

	cv::waitKey();
	return 0;
}