#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>

#include <vector>

using namespace std;

int main()
{
	cv::Mat image = cv::imread("building.jpg", 0);
	if (!image.data)return 0;
	cv::namedWindow("Image");
	cv::imshow("Image", image);

	// ������MSER�����
	cv::Ptr<cv::MSER> ptrMSER = cv::MSER::create(5,  // �ֲ����ʱʹ�õ�����ֵ
		200,	                                     // �������С��� 
		2000);                                       // �����������

	vector<vector<cv::Point> > points;  // �㼯������
	vector<cv::Rect> rects;  // ���ε�����
	ptrMSER->detectRegions(image, points, rects);  // ���MSER����

	cout << points.size() << " MSERs detected" << endl;

	// ������ɫͼ��
	cv::Mat output(image.size(), CV_8UC3);
	output = cv::Scalar(255, 255, 255);

	// OpenCV�����������
	cv::RNG rng;

	// ���ÿ����⵽�����������ڲ�ɫ������ʾMSER
	// ������������ʾ�ϴ��MSER
	for (vector<vector<cv::Point> >::reverse_iterator it = points.rbegin(); it != points.rend(); ++it) {
		// ���������ɫ
		cv::Vec3b c(rng.uniform(0, 254), rng.uniform(0, 254), rng.uniform(0, 254));

		cout << "MSER size = " << it->size() << endl;

		// ���MSER�����е�ÿ����
		for (vector<cv::Point>::iterator itPts = it->begin(); itPts != it->end(); ++itPts) {
			// ����дMSER������
			if (output.at<cv::Vec3b>(*itPts)[0] == 255) {
				output.at<cv::Vec3b>(*itPts) = c;
			}
		}
	}
	cv::namedWindow("MSER point sets");
	cv::imshow("MSER point sets", output);
	cv::imwrite("mser.bmp", output);


	// --------------------------------------------------
	// ��ȡ����ʾ���ε�MSER
	vector<cv::Rect>::iterator itr = rects.begin();
	vector<vector<cv::Point> >::iterator itp = points.begin();
	for (; itr != rects.end(); ++itr, ++itp) {
		// ������߱���
		if (static_cast<double>(itp->size()) / itr->area() > 0.6) {
			cv::rectangle(image, *itr, cv::Scalar(255), 2);
		}
	}
	cv::namedWindow("Rectangular MSERs");
	cv::imshow("Rectangular MSERs", image);


	// ���¼�������ͼ��
	image = cv::imread("building.jpg", 0);
	if (!image.data)return 0;
	
	// --------------------------------------------------
	// ��ȡ����ʾ��Բ�ε�MSER
	for (vector<vector<cv::Point> >::iterator it = points.begin(); it != points.end(); ++it) {
		// ����MSER�����е�ÿ����
		for (vector<cv::Point>::iterator itPts = it->begin(); itPts != it->end(); ++itPts) {
			// ��ȡ��յľ���
			cv::RotatedRect rr = cv::minAreaRect(*it);
			// �����Բ�ĳ����
			if (rr.size.height / rr.size.height > 0.6 || rr.size.height / rr.size.height < 1.6) {
				cv::ellipse(image, rr, cv::Scalar(255), 2);
			}
		}
	}
	cv::namedWindow("ellipses MSERs");
	cv::imshow("ellipses MSERs", image);

	cv::waitKey();
	return 0;
}