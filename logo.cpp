#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main() {
	namedWindow("Image");

	// 在感兴趣区域插入图片
	Mat image = imread("puppy.bmp");
	Mat logo = imread("smalllogo.png");  // logo的背景色为黑色

	// 在图像的右下角定义一个ROI（Region Of Interest）
	Mat imageROI(image,
		Rect(image.cols - logo.cols, image.rows - logo.rows,  // ROI坐标
			 logo.cols, logo.rows));						  // ROI大小

	logo.copyTo(imageROI);  // 插入logo

	imshow("Image", image);
	waitKey(0);

	
	// 利用掩码，在感兴趣区域插入logo时只复制不为黑色（即像素值不为0）的部分
	image = imread("puppy.bmp");  // 重新读取原图像	

	// 在图像的右下角定义一个ROI
	imageROI = image(Rect(image.cols - logo.cols, image.rows - logo.rows,
						  logo.cols, logo.rows));
	// 或使用ranges:
	// imageROI= image(Range(image.rows-logo.rows,image.rows), 
	//                 Range(image.cols-logo.cols,image.cols));

	Mat mask(logo);  // 把logo作为掩码（必须是灰度图像）

	logo.copyTo(imageROI, mask);  // 插入logo，只复制掩码不为0的位置

	imshow("Image", image);
	waitKey(0);

	return 0;
}