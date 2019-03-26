#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>

int main()
{
	cv::Mat image = cv::imread("binary.bmp");
	if (!image.data)return 0;
	cv::namedWindow("Image");
	cv::imshow("Image", image);

	// cv::erode()��ʴͼ��
	// ����Ĭ�ϵ�3x3�ṹԪ��
	cv::Mat eroded;  // Ŀ��ͼ��
	cv::erode(image, eroded, cv::Mat());
	// ��ʾ��ʴͼ��
	cv::namedWindow("Eroded Image");
	cv::imshow("Eroded Image", eroded);

	// cv::dilate����ͼ��
	cv::Mat dilated; // Ŀ��ͼ��
	cv::dilate(image, dilated, cv::Mat());
	// ��ʾ����ͼ��
	cv::namedWindow("Dilated Image");
	cv::imshow("Dilated Image", dilated);

	// ----------------------------------------
	// �ø���ĽṹԪ�ظ�ʴͼ��
	// ����7x7��mat����������ȫ��Ԫ�ض�Ϊ1
	cv::Mat element(7, 7, CV_8U, cv::Scalar(1));
	// ������ṹԪ�ظ�ʴͼ��
	cv::erode(image, eroded, element);
	cv::namedWindow("Eroded Image (7x7)");
	cv::imshow("Eroded Image (7x7)", eroded);

	// ----------------------------------------
	// ��ʴͼ������
	cv::erode(image, eroded, cv::Mat(), cv::Point(-1, -1), 3);
	cv::namedWindow("Eroded Image (3 times)");
	cv::imshow("Eroded Image (3 times)", eroded);

	// ----------------------------------------
	// �պ�ͼ�񣨶�ͼ�������ͺ�ʴ��
	cv::Mat element5(5, 5, CV_8U, cv::Scalar(1));
	cv::Mat closed;
	cv::morphologyEx(image, closed,  // ����������ͼ��
		cv::MORPH_CLOSE,             // �����
		element5);                   // �ṹԪ��
	// ��ʾ�պ�ͼ��
	cv::namedWindow("Closed Image");
	cv::imshow("Closed Image", closed);

	// ����ͼ�񣨶�ͼ���ȸ�ʴ�����ͣ�
	cv::Mat opened;
	cv::morphologyEx(image, opened, cv::MORPH_OPEN, element5);
	// ��ʾ����ͼ��
	cv::namedWindow("Opened Image");
	cv::imshow("Opened Image", opened);

	cv::waitKey();
	return 0;
}
