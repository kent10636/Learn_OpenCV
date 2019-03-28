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

	// 基本的MSER检测器
	cv::Ptr<cv::MSER> ptrMSER = cv::MSER::create(5,  // 局部检测时使用的增量值
		200,	                                     // 允许的最小面积 
		2000);                                       // 允许的最大面积

	vector<vector<cv::Point> > points;  // 点集的容器
	vector<cv::Rect> rects;  // 矩形的容器
	ptrMSER->detectRegions(image, points, rects);  // 检测MSER特征

	cout << points.size() << " MSERs detected" << endl;

	// 创建白色图像
	cv::Mat output(image.size(), CV_8UC3);
	output = cv::Scalar(255, 255, 255);

	// OpenCV随机数生成器
	cv::RNG rng;

	// 针对每个检测到的特征区域，在彩色区域显示MSER
	// 反向排序，先显示较大的MSER
	for (vector<vector<cv::Point> >::reverse_iterator it = points.rbegin(); it != points.rend(); ++it) {
		// 生成随机颜色
		cv::Vec3b c(rng.uniform(0, 254), rng.uniform(0, 254), rng.uniform(0, 254));

		cout << "MSER size = " << it->size() << endl;

		// 针对MSER集合中的每个点
		for (vector<cv::Point>::iterator itPts = it->begin(); itPts != it->end(); ++itPts) {
			// 不重写MSER的像素
			if (output.at<cv::Vec3b>(*itPts)[0] == 255) {
				output.at<cv::Vec3b>(*itPts) = c;
			}
		}
	}
	cv::namedWindow("MSER point sets");
	cv::imshow("MSER point sets", output);
	cv::imwrite("mser.bmp", output);


	// --------------------------------------------------
	// 提取并显示矩形的MSER
	vector<cv::Rect>::iterator itr = rects.begin();
	vector<vector<cv::Point> >::iterator itp = points.begin();
	for (; itr != rects.end(); ++itr, ++itp) {
		// 检查两者比例
		if (static_cast<double>(itp->size()) / itr->area() > 0.6) {
			cv::rectangle(image, *itr, cv::Scalar(255), 2);
		}
	}
	cv::namedWindow("Rectangular MSERs");
	cv::imshow("Rectangular MSERs", image);


	// 重新加载输入图像
	image = cv::imread("building.jpg", 0);
	if (!image.data)return 0;
	
	// --------------------------------------------------
	// 提取并显示椭圆形的MSER
	for (vector<vector<cv::Point> >::iterator it = points.begin(); it != points.end(); ++it) {
		// 遍历MSER集合中的每个点
		for (vector<cv::Point>::iterator itPts = it->begin(); itPts != it->end(); ++itPts) {
			// 提取封闭的矩形
			cv::RotatedRect rr = cv::minAreaRect(*it);
			// 检查椭圆的长宽比
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