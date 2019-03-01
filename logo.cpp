#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main() {
	namedWindow("Image");

	// �ڸ���Ȥ�������ͼƬ
	Mat image = imread("puppy.bmp");
	Mat logo = imread("smalllogo.png");  // logo�ı���ɫΪ��ɫ

	// ��ͼ������½Ƕ���һ��ROI��Region Of Interest��
	Mat imageROI(image,
		Rect(image.cols - logo.cols, image.rows - logo.rows,  // ROI����
			 logo.cols, logo.rows));						  // ROI��С

	logo.copyTo(imageROI);  // ����logo

	imshow("Image", image);
	waitKey(0);

	
	// �������룬�ڸ���Ȥ�������logoʱֻ���Ʋ�Ϊ��ɫ��������ֵ��Ϊ0���Ĳ���
	image = imread("puppy.bmp");  // ���¶�ȡԭͼ��	

	// ��ͼ������½Ƕ���һ��ROI
	imageROI = image(Rect(image.cols - logo.cols, image.rows - logo.rows,
						  logo.cols, logo.rows));
	// ��ʹ��ranges:
	// imageROI= image(Range(image.rows-logo.rows,image.rows), 
	//                 Range(image.cols-logo.cols,image.cols));

	Mat mask(logo);  // ��logo��Ϊ���루�����ǻҶ�ͼ��

	logo.copyTo(imageROI, mask);  // ����logo��ֻ�������벻Ϊ0��λ��

	imshow("Image", image);
	waitKey(0);

	return 0;
}