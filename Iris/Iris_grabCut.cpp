// 尝试使用grabCut进行虹膜分割
// 能进行基本分割，但效果一般，光斑、睫毛未去除，圆的边缘不光滑
// 需手动定位矩形区域

#include <iostream>

#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>

using namespace std;

int main()
{
	cv::Mat image = cv::imread("S6000S00.jpg");
	if (!image.data)return 0;
	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);

	cv::Rect rectangle1(image.cols / 2 - 200, image.rows / 2 - 200, 400, 400);
	cv::Rect rectangle2(image.cols / 2 - 85, image.rows / 2 - 85, 170, 170);
	cv::Mat bgModel1, fgModel1;
	cv::Mat bgModel2, fgModel2;
	cv::Mat result1;
	cv::Mat result2;

	// ---------- grabCut分割外圆 ----------
	cv::grabCut(image,           // 输入图像
		result1,                 // 分割结果
		rectangle1,              // 包含前景的矩形 
		bgModel1, fgModel1,      // 模型
		1,                       // 迭代次数
		cv::GC_INIT_WITH_RECT);  // 使用矩形
	// -------------------------------------

	// ---------- grabCut分割内圆 ----------
	cv::grabCut(image,           // 输入图像
		result2,                 // 分割结果
		rectangle2,              // 包含前景的矩形 
		bgModel2, fgModel2,      // 模型
		1,                       // 迭代次数
		cv::GC_INIT_WITH_RECT);  // 使用矩形
	// -------------------------------------
	
	cv::rectangle(image, rectangle1, cv::Scalar(255, 255, 255), 1);
	cv::namedWindow("Image With Rectangle");
	cv::imshow("Image With Rectangle", image);

	// cv::GC_BGD 明确属于背景的像素
	// cv::GC_FGD 明确属于前景的像素
	// cv::GC_PR_BGD 可能属于背景的像素
	// cv::GC_PR_FGD 可能属于前景的像素

	// 外圆
	cv::compare(result1, cv::GC_PR_FGD, result1, cv::CMP_EQ);
	cv::Mat object1(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));
	image.copyTo(object1, result1);
	cv::namedWindow("Object1");
	cv::imshow("Object1", object1);
	
	// 内圆
	cv::compare(result2, cv::GC_PR_FGD, result2, cv::CMP_EQ);
	cv::Mat object2(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));
	image.copyTo(object2, result2);
	cv::namedWindow("Object2");
	cv::imshow("Object2", object2);
	
	// 虹膜，外圆部分减去内圆部分
	cv::Mat dst = result1 - result2;
	cv::Mat object3(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));  // 创建一个白色图像
	image.copyTo(object3, dst);  // 不复制背景像素
	cv::namedWindow("Result");
	cv::imshow("Result", object3);

	cv::waitKey();
	return 0;
}