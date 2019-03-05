#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

void sharpen(const cv::Mat &image, cv::Mat &result) {
	// 判断是否需要分配图像数据，若需要，则分配
	result.create(image.size(), image.type());
	int nchannels = image.channels();  // 获得通道数

	// 处理所有行（除了第一行和最后一行）
	for (int j = 1; j < image.rows - 1; j++) {
		const uchar* previous = image.ptr<const uchar>(j - 1);  // 上一行
		const uchar* current = image.ptr<const uchar>(j);	    // 当前行
		const uchar* next = image.ptr<const uchar>(j + 1);		// 下一行

		uchar* output = result.ptr<uchar>(j);	// 输出行

		for (int i = nchannels; i < (image.cols - 1)*nchannels; i++) {
			// 应用锐化算子
			*output++ = saturate_cast<uchar>(5 * current[i] - current[i - nchannels] - current[i + nchannels] - previous[i] - next[i]);
		}
	}

	// 把未处理的像素设为0
	result.row(0).setTo(cv::Scalar(0));
	result.row(result.rows - 1).setTo(cv::Scalar(0));
	result.col(0).setTo(cv::Scalar(0));
	result.col(result.cols - 1).setTo(cv::Scalar(0));
}

void sharpen2D(const cv::Mat &image, cv::Mat &result) {  // 锐化滤波器

	// 构造内核（所有入口都初始化为0）
	cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));
	// 对内核赋值
	kernel.at<float>(1, 1) = 5.0;
	kernel.at<float>(0, 1) = -1.0;
	kernel.at<float>(2, 1) = -1.0;
	kernel.at<float>(1, 0) = -1.0;
	kernel.at<float>(1, 2) = -1.0;

	// 对图像滤波
	cv::filter2D(image, result, image.depth(), kernel);
}

int main() {
	Mat image = imread("boldt.jpg");
	if (!image.data)return 0;
	
	Mat result;
	
	double time = static_cast<double>(getTickCount());

	sharpen(image, result);
	//sharpen2D(image, result);

	time = (static_cast<double>(getTickCount()) - time) / getTickFrequency();
	cout << "time= " << time << endl;

	namedWindow("Image");
	imshow("Image", image);
	waitKey(0);

	return true;
}