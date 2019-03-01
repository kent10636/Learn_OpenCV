#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace std;

void onMouse(int event, int x, int y, int flags, void* param) {
	cv::Mat *im = reinterpret_cast<cv::Mat*>(param);
	switch (event) {  // �����¼�
	case cv::EVENT_LBUTTONDOWN:  // �����������¼�
		cout << "at (" << x << "," << y << ") value is: " << static_cast<int>(im->at<uchar>(cv::Point(x, y))) << endl;  // ��ʾ����ֵ(x,y)
		break;
	}
}

int main() {
	cv::Mat image;  // ����һ����ͼ��
	//cout << "This image is " << image.rows << " x " << image.cols << endl << endl;

	image = cv::imread("puppy.bmp");  // ��ȡ����ͼ��
	//image = cv::imread("puppy.bmp", cv::IMREAD_GRAYSCALE);  // ��ȡͼ�񣬲�����ת��Ϊ�Ҷ�ͼ��
	//image = cv::imread("puppy.bmp", cv::IMREAD_COLOR);  // ��ȡͼ�񣬲�����ת��Ϊ��ͨ����ɫͼ��

	if (image.empty()) {  // ������
		cout << "Error reading image." << endl;
		return 0;
	}

	cout << "This image is " << image.rows << " x " << image.cols << endl;  // ����ͼ��ĳߴ�
	cout << "This image has " << image.channels() << " channel(s)" << endl;  // ����ͼ���ͨ����

	cv::namedWindow("Original Image");  // ���崰��
	cv::imshow("Original Image", image);  // ��ʾͼ��

	cv::setMouseCallback("Original Image", onMouse, reinterpret_cast<void*>(&image));

	cv::Mat result;  // ������һ����ͼ��
	cv::flip(image, result, 1);  // ��תͼ��������ʾˮƽ��0��ʾ��ֱ��������ʾˮƽ�ʹ�ֱ��
	//cv::flip(image, result, 1);  // �͵ش���

	cv::namedWindow("Output Image");  // �������
	cv::imshow("Output Image", result);
	cv::waitKey(0);  // 0��ʾһֱ�ȴ�������������ʾ�ȴ��ĺ�����
	cv::imwrite("output.bmp", result);  // ������


	cv::namedWindow("Drawing on an Image");

	cv::circle(image,         // Ŀ��ͼ��
		cv::Point(260, 125),  // ���ĵ�����
		120,                   // �뾶
		0,                    // ��ɫ���˴��ú�ɫ��
		3);                   // ���

	cv::putText(image,           // Ŀ��ͼ��
		"This is a dog.",        // �ı�
		cv::Point(40, 200),      // �ı�λ��
		cv::FONT_HERSHEY_PLAIN,  // ��������
		2.0,                     // �����С
		255,                     // �ı���ɫ���˴��ð�ɫ��
		2);                      // �ı����

	cv::imshow("Drawing on an Image", image);
	cv::waitKey(0);

	return 0;
}