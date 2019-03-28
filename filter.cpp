#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

int main()
{
	cv::Mat image = cv::imread("boldt.jpg", 0);
	if (!image.data)return 0;
	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);

	// --------------------------------------------------
	// ��һ����ֵ�˲���ģ��ͼ��
	cv::Mat result;
	cv::blur(image, result, cv::Size(5, 5));
	cv::namedWindow("Mean filtered Image");
	cv::imshow("Mean filtered Image", result);

	// --------------------------------------------------
	// ��һ��9x9��ֵ�˲���ģ��ͼ��
	cv::blur(image, result, cv::Size(9, 9));
	cv::namedWindow("Mean filtered Image (9x9)");
	cv::imshow("Mean filtered Image (9x9)", result);

	// --------------------------------------------------
	// ��˹����ģ��ͼ��
	cv::GaussianBlur(image, result,
		cv::Size(5, 5),  // �˲����ߴ�
		1.5);			 // ���Ƹ�˹������״�Ĳ���
	cv::namedWindow("Gaussian filtered Image");
	cv::imshow("Gaussian filtered Image", result);

	cv::GaussianBlur(image, result, cv::Size(9, 9), 1.7);
	cv::namedWindow("Gaussian filtered Image (9x9)");
	cv::imshow("Gaussian filtered Image (9x9)", result);

	// --------------------------------------------------
	// �����˹�˲����Ĺ�һ��ϵ��
	// ���ݸ�˹������sigmaֵ����ȡ��˹�˲����ں� (sigma = 1.5)
	cv::Mat gauss = cv::getGaussianKernel(9, 1.5, CV_32F);
	cv::Mat_<float>::const_iterator it = gauss.begin<float>();
	cv::Mat_<float>::const_iterator itend = gauss.end<float>();
	std::cout << "1.5 = [";
	for (; it != itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;

	// ���ݸ�˹������sigmaֵ����ȡ��˹�˲����ں� (sigma = 0.5)
	gauss = cv::getGaussianKernel(9, 0.5, CV_32F);
	it = gauss.begin<float>();
	itend = gauss.end<float>();
	std::cout << "0.5 = [";
	for (; it != itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;

	// ���ݸ�˹������sigmaֵ����ȡ��˹�˲����ں� (sigma = 2.5)
	gauss = cv::getGaussianKernel(9, 2.5, CV_32F);
	it = gauss.begin<float>();
	itend = gauss.end<float>();
	std::cout << "2.5 = [";
	for (; it != itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;

	// ��ȡ��˹�˲����ں� (9 elements)
	gauss = cv::getGaussianKernel(9, -1, CV_32F);
	it = gauss.begin<float>();
	itend = gauss.end<float>();
	std::cout << "9 = [";
	for (; it != itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;

	// --------------------------------------------------
	// �������ز���
	// ��ͼ��ߴ���С��1/4(����ʽ��ֻ����ÿ4��(��)�����е�1��(��)����Ƶ�ɷ�δȥ����������ԣ�ͼ��Ч����)
	image = cv::imread("boldt.jpg", 0);
	cv::Mat reduced(image.rows / 4, image.cols / 4, CV_8U);
	for (int i = 0; i<reduced.rows; i++)
		for (int j = 0; j<reduced.cols; j++)
			reduced.at<uchar>(i, j) = image.at<uchar>(i * 4, j * 4);
	cv::resize(reduced, reduced, cv::Size(), 4, 4, cv::INTER_NEAREST);  //�Ŵ��ı���ʾ
	cv::namedWindow("Badly Reduced Image");
	cv::imshow("Badly Reduced Image", reduced);

	// ����ȥ����Ƶ�ɷ�(��ȷ��ʽ)
	cv::GaussianBlur(image, image, cv::Size(11, 11), 1.75);
	// ֻ����ÿ4�������е�1��
	cv::Mat reduced2(image.rows / 4, image.cols / 4, CV_8U);
	for (int i = 0; i<reduced2.rows; i++)
		for (int j = 0; j<reduced2.cols; j++)
			reduced2.at<uchar>(i, j) = image.at<uchar>(i * 4, j * 4);
	cv::Mat newImage;
	
	cv::resize(reduced2, newImage, cv::Size(), 4, 4, cv::INTER_NEAREST);  //�Ŵ��ı���ʾ(���ڽ���ֵ)
	cv::namedWindow("Reduced Image(Nearest Resizing)");
	cv::imshow("Reduced Image(Nearest Resizing)", newImage);

	cv::resize(reduced2, newImage, cv::Size(), 4, 4, cv::INTER_LINEAR);  //�Ŵ��ı���ʾ(˫���Բ�ֵ)
	cv::namedWindow("Reduced Image(Bilinear Resizing)");
	cv::imshow("Reduced Image(Bilinear Resizing)", newImage);

	// --------------------------------------------------
	// ������������
	image = cv::imread("salted.bmp", 0);
	if (!image.data)return 0;
	cv::namedWindow("S&P Image");
	cv::imshow("S&P Image", image);

	// ʹ�þ�ֵ�˲������޷�����������
	cv::blur(image, result, cv::Size(5, 5));
	cv::namedWindow("Mean Filtered Image");
	cv::imshow("Mean Filtered Image", result);

	// ʹ����ֵ�˲������ɹ�����������
	cv::medianBlur(image, result, 5);  // ���һ���������˲����ߴ�
	cv::namedWindow("Median Filtered Image");
	cv::imshow("Median Filtered Image", result);

	cv::waitKey();
	return 0;
}