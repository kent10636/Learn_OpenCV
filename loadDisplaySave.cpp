#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace std;

void onMouse(int event, int x, int y, int flags, void* param) {
	cv::Mat *im = reinterpret_cast<cv::Mat*>(param);
	switch (event) {  // 调度事件
	case cv::EVENT_LBUTTONDOWN:  // 鼠标左键按下事件
		cout << "at (" << x << "," << y << ") value is: " << static_cast<int>(im->at<uchar>(cv::Point(x, y))) << endl;  // 显示像素值(x,y)
		break;
	}
}

int main() {
	cv::Mat image;  // 创建一个空图像
	//cout << "This image is " << image.rows << " x " << image.cols << endl << endl;

	image = cv::imread("puppy.bmp");  // 读取输入图像
	//image = cv::imread("puppy.bmp", cv::IMREAD_GRAYSCALE);  // 读取图像，并将其转换为灰度图像
	//image = cv::imread("puppy.bmp", cv::IMREAD_COLOR);  // 读取图像，并将其转换为三通道彩色图像

	if (image.empty()) {  // 错误处理
		cout << "Error reading image." << endl;
		return 0;
	}

	cout << "This image is " << image.rows << " x " << image.cols << endl;  // 输入图像的尺寸
	cout << "This image has " << image.channels() << " channel(s)" << endl;  // 输入图像的通道数

	cv::namedWindow("Original Image");  // 定义窗口
	cv::imshow("Original Image", image);  // 显示图像

	cv::setMouseCallback("Original Image", onMouse, reinterpret_cast<void*>(&image));

	cv::Mat result;  // 创建另一个空图像
	cv::flip(image, result, 1);  // 翻转图像（正数表示水平，0表示垂直，负数表示水平和垂直）
	//cv::flip(image, result, 1);  // 就地处理

	cv::namedWindow("Output Image");  // 输出窗口
	cv::imshow("Output Image", result);
	cv::waitKey(0);  // 0表示一直等待按键，正数表示等待的毫秒数
	cv::imwrite("output.bmp", result);  // 保存结果


	cv::namedWindow("Drawing on an Image");

	cv::circle(image,         // 目标图像
		cv::Point(260, 125),  // 中心点坐标
		120,                   // 半径
		0,                    // 颜色（此处用黑色）
		3);                   // 厚度

	cv::putText(image,           // 目标图像
		"This is a dog.",        // 文本
		cv::Point(40, 200),      // 文本位置
		cv::FONT_HERSHEY_PLAIN,  // 字体类型
		2.0,                     // 字体大小
		255,                     // 文本颜色（此处用白色）
		2);                      // 文本厚度

	cv::imshow("Drawing on an Image", image);
	cv::waitKey(0);

	return 0;
}