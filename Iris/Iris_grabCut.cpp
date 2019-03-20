// ����ʹ��grabCut���к�Ĥ�ָ�
// �ܽ��л����ָ��Ч��һ�㣬��ߡ���ëδȥ����Բ�ı�Ե���⻬
// ���ֶ���λ��������

#include <iostream>

#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>

using namespace std;

int main()
{
	cv::Mat image = cv::imread("S6000S00.jpg");
	if (!image.data)return 0;
	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);

	cv::Rect rectangle1(image.cols / 2 - 200, image.rows / 2 - 200, 400, 400);
	cv::Rect rectangle2(image.cols / 2 - 85, image.rows / 2 - 85, 170, 170);
	cv::Mat bgModel1, fgModel1;
	cv::Mat bgModel2, fgModel2;
	cv::Mat result1;
	cv::Mat result2;

	// ---------- grabCut�ָ���Բ ----------
	cv::grabCut(image,           // ����ͼ��
		result1,                 // �ָ���
		rectangle1,              // ����ǰ���ľ��� 
		bgModel1, fgModel1,      // ģ��
		1,                       // ��������
		cv::GC_INIT_WITH_RECT);  // ʹ�þ���
	// -------------------------------------

	// ---------- grabCut�ָ���Բ ----------
	cv::grabCut(image,           // ����ͼ��
		result2,                 // �ָ���
		rectangle2,              // ����ǰ���ľ��� 
		bgModel2, fgModel2,      // ģ��
		1,                       // ��������
		cv::GC_INIT_WITH_RECT);  // ʹ�þ���
	// -------------------------------------
	
	cv::rectangle(image, rectangle1, cv::Scalar(255, 255, 255), 1);
	cv::namedWindow("Image With Rectangle");
	cv::imshow("Image With Rectangle", image);

	// cv::GC_BGD ��ȷ���ڱ���������
	// cv::GC_FGD ��ȷ����ǰ��������
	// cv::GC_PR_BGD �������ڱ���������
	// cv::GC_PR_FGD ��������ǰ��������

	// ��Բ
	cv::compare(result1, cv::GC_PR_FGD, result1, cv::CMP_EQ);
	cv::Mat object1(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));
	image.copyTo(object1, result1);
	cv::namedWindow("Object1");
	cv::imshow("Object1", object1);
	
	// ��Բ
	cv::compare(result2, cv::GC_PR_FGD, result2, cv::CMP_EQ);
	cv::Mat object2(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));
	image.copyTo(object2, result2);
	cv::namedWindow("Object2");
	cv::imshow("Object2", object2);
	
	// ��Ĥ����Բ���ּ�ȥ��Բ����
	cv::Mat dst = result1 - result2;
	cv::Mat object3(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));  // ����һ����ɫͼ��
	image.copyTo(object3, dst);  // �����Ʊ�������
	cv::namedWindow("Result");
	cv::imshow("Result", object3);

	cv::waitKey();
	return 0;
}