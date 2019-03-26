#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>

int main()
{
	cv::Mat image = cv::imread("binary.bmp");
	if (!image.data)return 0;
	cv::namedWindow("Image");
	cv::imshow("Image", image);

	// cv::erode()腐蚀图像
	// 采用默认的3x3结构元素
	cv::Mat eroded;  // 目标图像
	cv::erode(image, eroded, cv::Mat());
	// 显示腐蚀图像
	cv::namedWindow("Eroded Image");
	cv::imshow("Eroded Image", eroded);

	// cv::dilate膨胀图像
	cv::Mat dilated; // 目标图像
	cv::dilate(image, dilated, cv::Mat());
	// 显示膨胀图像
	cv::namedWindow("Dilated Image");
	cv::imshow("Dilated Image", dilated);

	// ----------------------------------------
	// 用更大的结构元素腐蚀图像
	// 创建7x7的mat变量，其中全部元素都为1
	cv::Mat element(7, 7, CV_8U, cv::Scalar(1));
	// 用这个结构元素腐蚀图像
	cv::erode(image, eroded, element);
	cv::namedWindow("Eroded Image (7x7)");
	cv::imshow("Eroded Image (7x7)", eroded);

	// ----------------------------------------
	// 腐蚀图像三次
	cv::erode(image, eroded, cv::Mat(), cv::Point(-1, -1), 3);
	cv::namedWindow("Eroded Image (3 times)");
	cv::imshow("Eroded Image (3 times)", eroded);

	// ----------------------------------------
	// 闭合图像（对图像先膨胀后腐蚀）
	cv::Mat element5(5, 5, CV_8U, cv::Scalar(1));
	cv::Mat closed;
	cv::morphologyEx(image, closed,  // 输入和输出的图像
		cv::MORPH_CLOSE,             // 运算符
		element5);                   // 结构元素
	// 显示闭合图像
	cv::namedWindow("Closed Image");
	cv::imshow("Closed Image", closed);

	// 开启图像（对图像先腐蚀后膨胀）
	cv::Mat opened;
	cv::morphologyEx(image, opened, cv::MORPH_OPEN, element5);
	// 显示开启图像
	cv::namedWindow("Opened Image");
	cv::imshow("Opened Image", opened);

	cv::waitKey();
	return 0;
}
