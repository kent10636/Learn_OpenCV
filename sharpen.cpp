#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

void sharpen(const cv::Mat &image, cv::Mat &result) {
	// �ж��Ƿ���Ҫ����ͼ�����ݣ�����Ҫ�������
	result.create(image.size(), image.type());
	int nchannels = image.channels();  // ���ͨ����

	// ���������У����˵�һ�к����һ�У�
	for (int j = 1; j < image.rows - 1; j++) {
		const uchar* previous = image.ptr<const uchar>(j - 1);  // ��һ��
		const uchar* current = image.ptr<const uchar>(j);	    // ��ǰ��
		const uchar* next = image.ptr<const uchar>(j + 1);		// ��һ��

		uchar* output = result.ptr<uchar>(j);	// �����

		for (int i = nchannels; i < (image.cols - 1)*nchannels; i++) {
			// Ӧ��������
			*output++ = saturate_cast<uchar>(5 * current[i] - current[i - nchannels] - current[i + nchannels] - previous[i] - next[i]);
		}
	}

	// ��δ�����������Ϊ0
	result.row(0).setTo(cv::Scalar(0));
	result.row(result.rows - 1).setTo(cv::Scalar(0));
	result.col(0).setTo(cv::Scalar(0));
	result.col(result.cols - 1).setTo(cv::Scalar(0));
}

int main() {
	Mat image = imread("boldt.jpg");
	if (!image.data)return 0;
	
	Mat result;
	
	double time = static_cast<double>(getTickCount());

	sharpen(image, result);

	time = (static_cast<double>(getTickCount()) - time) / getTickFrequency();
	cout << "time= " << time << endl;

	namedWindow("Image");
	imshow("Image", image);
	waitKey(0);

	return true;
}