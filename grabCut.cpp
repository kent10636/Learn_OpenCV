#include <iostream>

#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>

using namespace std;

int main()
{
	// ---------- �ָ��ȡ���� ----------
	cv::Mat image = cv::imread("boldt.jpg");
	if (!image.data)return 0;
	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);

	cv::Rect rectangle(50, 25, 350, 280);  // ����һ�����߿�ľ��Σ������ⲿ�����ؽ��ᱻ���Ϊ����
	cv::Mat bgModel, fgModel;  // ģ�ͣ��ڲ�ʹ�ã�
	cv::Mat result;  // �ָ�����4�ֿ��ܵ�ֵ��

	// grabCut�ָ��㷨
	cv::grabCut(image,           // ����ͼ��
		result,                  // �ָ���
		rectangle,               // ����ǰ���ľ��� 
		bgModel, fgModel,        // ģ��
		5,                       // ��������
		cv::GC_INIT_WITH_RECT);  // ʹ�þ���

	cv::compare(result, cv::GC_PR_FGD, result, cv::CMP_EQ);  // ȡ�ñ��Ϊ����������ǰ����������
	cv::Mat foreground(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));  // ����һ����ɫͼ��
	image.copyTo(foreground, result);  // �����Ʊ�������

	// ��ԭͼ���ϻ��ƾ���
	cv::rectangle(image, rectangle, cv::Scalar(255, 255, 255), 1);
	
	cv::namedWindow("Image With Rectangle");
	cv::imshow("Image With Rectangle", image);

	// ��ʾ���
	cv::namedWindow("Foreground Object");
	cv::imshow("Foreground Object", foreground);

	cv::waitKey();
	return 0;
}