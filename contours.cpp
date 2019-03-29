#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "linefinder.h"
#include "edgedetector.h"

#define PI 3.1415926

using namespace std;

int main()
{
	cv::Mat image = cv::imread("road.jpg", 0);
	if (!image.data)return 0;
	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);

	// 计算Sobel算子
	EdgeDetector ed;
	ed.computeSobel(image);

	// Display the Sobel orientation
	cv::namedWindow("Sobel (orientation)");
	cv::imshow("Sobel (orientation)", ed.getSobelOrientationImage());
	//cv::imwrite("ori.bmp", ed.getSobelOrientationImage());

	// Display the Sobel(低阈值)
	cv::namedWindow("Sobel (low threshold)");
	cv::imshow("Sobel (low threshold)", ed.getBinaryMap(125));

	// Display the Sobel(高阈值)
	cv::namedWindow("Sobel (high threshold)");
	cv::imshow("Sobel (high threshold)", ed.getBinaryMap(350));

	// --------------------------------------------------
	// 应用Canny算法
	cv::Mat contours;
	cv::Canny(image,  // 灰度图像
		contours,     // 输出轮廓
		125,          // 低阈值
		350);         // 高阈值

	// Display the image of contours
	cv::namedWindow("Canny Contours");
	cv::imshow("Canny Contours", 255 - contours);

	// --------------------------------------------------
	// 用霍夫变换(Hough tranform)检测直线
	std::vector<cv::Vec2f> lines;
	cv::HoughLines(contours, lines, 1, 
		PI / 180,  // 步长，表示函数将搜索所有可能的角度
		50);       // 最小投票数

	// 画出直线
	cv::Mat result(contours.rows, contours.cols, CV_8U, cv::Scalar(255));
	image.copyTo(result);
	std::cout << "Lines detected: " << lines.size() << std::endl;

	std::vector<cv::Vec2f>::const_iterator it = lines.begin();
	while (it != lines.end()) {
		float rho = (*it)[0];    // 第一个元素是距离rho
		float theta = (*it)[1];  // 第二个元素是角度theta

		if (theta < PI / 4. || theta > 3.*PI / 4.) {  // 垂直线（大致）
			cv::Point pt1(rho / cos(theta), 0);  // 直线与第一行的交叉点	
			cv::Point pt2((rho - result.rows*sin(theta)) / cos(theta), result.rows);  // 直线与最后一行的交叉点
			cv::line(result, pt1, pt2, cv::Scalar(255), 1);  // 画白色的线
		}
		else {  // 水平线（大致）			
			cv::Point pt1(0, rho / sin(theta));  // 直线与第一列的交叉点		
			cv::Point pt2(result.cols, (rho - result.cols*cos(theta)) / sin(theta));  // 直线与最后一列的交叉点
			cv::line(result, pt1, pt2, cv::Scalar(255), 1);  // 画白色的线
		}
		std::cout << "line: (" << rho << "," << theta << ")\n";
		++it;
	}

	// Display the detected line image
	cv::namedWindow("Lines with Hough");
	cv::imshow("Lines with Hough", result);

	// --------------------------------------------------
	// 创建LineFinder类的实例
	LineFinder ld;

	// 设置概率霍夫变换的参数
	ld.setLineLengthAndGap(100, 20);
	ld.setMinVote(60);

	// 检测直线并画线
	std::vector<cv::Vec4i> li = ld.findLines(contours);
	ld.drawDetectedLines(image);
	cv::namedWindow("Lines with HoughP");
	cv::imshow("Lines with HoughP", image);

	// --------------------------------------------------
	// 检测圆
	image = cv::imread("chariot.jpg", 0);

	cv::GaussianBlur(image, image, cv::Size(5, 5), 1.5);
	std::vector<cv::Vec3f> circles;  // 存放检测结果
	// cv::HoughCircles函数将Canny检测与霍夫变换结合，调用之前要对图像进行平滑化，以减少噪声
	cv::HoughCircles(image, circles, cv::HOUGH_GRADIENT,
		2,        // 累加器分辨率 (图像尺寸 / 2) 
		20,       // 两个圆之间的最小距离
		200,      // Canny算子的高阈值
		60,       // 最少投票数
		15, 50);  // 最小和最大半径

	std::cout << "Circles: " << circles.size() << std::endl;

	// 画圆
	image = cv::imread("chariot.jpg", 0);
	std::vector<cv::Vec3f>::const_iterator itc = circles.begin();
	while (itc != circles.end()) {
		cv::circle(image,
			cv::Point((*itc)[0], (*itc)[1]), // 圆心
			(*itc)[2],                       // 半径
			cv::Scalar(255),                 // 颜色 
			2);                              // 厚度

		++itc;
	}
	cv::namedWindow("Detected Circles");
	cv::imshow("Detected Circles", image);

	cv::waitKey();
	return 0;
}