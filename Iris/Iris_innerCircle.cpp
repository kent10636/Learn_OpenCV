#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/types_c.h>

using namespace std;

int main() {
	string imageFile = "./iris/S6753S07.jpg";  // 图像文件路径

	cv::Mat srcImage = cv::imread(imageFile, -1);
	if (!srcImage.data)return 0;
	cv::namedWindow("Source Image");
	cv::imshow("Source Image", srcImage);

	cv::Mat image;  // 存放处理过程中的图像

	int srcChannels = srcImage.channels();
	//cout << "Channels: " << srcChannels << endl;
	if (srcChannels != 1) {  // 若原图像不是灰度图，则将其灰度化
		cv::Mat grayImage;
		cv::cvtColor(srcImage, grayImage, CV_RGB2GRAY);
		image = grayImage;
		cv::namedWindow("Gray Image");
		cv::imshow("Gray Image", grayImage);
	}
	else {
		image = srcImage;
	}

	/*
	//将原图像灰度化
	if (srcImage.type() == CV_8UC1) {  // 灰度图
		image = srcImage;
	}
	else if (image.type() == CV_8UC3) {  // 彩色图
		cv::Mat grayImage;
		cv::cvtColor(srcImage, grayImage, CV_RGB2GRAY);
		image = grayImage;
		cv::namedWindow("Gray Image");
		cv::imshow("Gray Image", grayImage);
	}
	*/

	// --------------------------------------------------
	// 检测图像轮廓，以便调整cv::HoughCircles函数中的阈值
	cv::Mat contours;
	cv::Canny(image,  // 灰度图像
		contours,     // 输出轮廓
		70,           // 低阈值
		140);         // 高阈值

	// 显示图像边缘轮廓
	cv::namedWindow("Canny Contours");
	cv::imshow("Canny Contours", 255 - contours);

	// --------------------------------------------------
	// 虹膜内圆检测
	cv::GaussianBlur(image, image, cv::Size(5, 5), 1.5);  // 高斯模糊/高斯平滑，减少图像噪声，降低细节层次
	//cv::namedWindow("Blured Image");
	//cv::imshow("Blured Image", image);

	std::vector<cv::Vec3f> circles;  // 用于存放检测结果
	
	// 第一次应用cv::HoughCircles函数时使用的参数
	int canny_threshold = 140, 
		vote = 100, 
		min_rad = 10, max_rad = 80;
	// 第一次应用cv::HoughCircles函数
	cv::HoughCircles(image, circles, cv::HOUGH_GRADIENT,
		2,                  // 累加器分辨率 (图像尺寸 / 2) 
		20,                 // 两个圆之间的最小距离
		canny_threshold,    // Canny算子的高阈值，阈值高可产生更明确的边缘
		vote,               // 最少投票数，投票数高可产生更准确的圆
		min_rad, max_rad);  // 最小和最大半径
	int circles_num = circles.size();
	std::cout << "Circles: " << circles.size() << std::endl;

	if (circles_num > 1) {  // 第一次时检测到2个及以上的圆
		for (; circles_num > 1; vote += 10) {
			cv::HoughCircles(image, circles, cv::HOUGH_GRADIENT,
				2,
				20,
				canny_threshold,
				vote,
				min_rad, max_rad);
			circles_num = circles.size();
			std::cout << "Circles: " << circles_num << std::endl;
		}
	}
	else if (circles_num == 0) {  // 第一次时检测不到圆
		for (; circles_num == 0; vote -= 10) {
			cv::HoughCircles(image, circles, cv::HOUGH_GRADIENT,
				2,
				20,
				canny_threshold,
				vote,
				min_rad, max_rad);
			circles_num = circles.size();
			std::cout << "Circles: " << circles_num << std::endl;
		}
	}

	// 画圆
	image = cv::imread(imageFile, -1);
	std::vector<cv::Vec3f>::const_iterator itc = circles.begin();
	while (itc != circles.end()) {
		cv::circle(image,
			cv::Point((*itc)[0], (*itc)[1]),  // 圆心
			(*itc)[2],                        // 半径
			cv::Scalar(255),                  // 颜色 
			2);                               // 厚度

		++itc;
	}
	cv::namedWindow("Detected Circles");
	cv::imshow("Detected Circles", image);

	cv::waitKey();
	return 0;
}