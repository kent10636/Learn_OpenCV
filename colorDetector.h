#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class ColorDetector {
private:
	int maxDist;  // �������С���
	cv::Vec3b target;  // Ŀ����ɫ
	cv::Mat result;  // �洢��ֵӳ������ͼ��

public:
	// �չ��캯�����ڴ˳�ʼ��Ĭ�ϲ���
	ColorDetector() : maxDist(100), target(0, 0, 0) {}

	// ������Ŀ����ɫ�Ĳ��
	int getDistanceToTargetColor(const cv::Vec3b& color) const {
		return getColorDistance(color, target);
	}

	// ����������ɫ֮��ĳ�������
	int getColorDistance(const cv::Vec3b& color1, const cv::Vec3b& color2) const {

		return abs(color1[0] - color2[0]) +
			abs(color1[1] - color2[1]) +
			abs(color1[2] - color2[2]);

		// Or:
		// return static_cast<int>(cv::norm<int,3>(cv::Vec3i(color[0]-color2[0],color[1]-color2[1],color[2]-color2[2])));

		// Or:
		// cv::Vec3b dist;
		// cv::absdiff(color,color2,dist);
		// return cv::sum(dist)[0];
	}

	// ����ͼ�񣬷��ص�ͨ����ֵͼ��
	cv::Mat process(const cv::Mat &image)
	{

		// ��Ҫʱ���·����ֵӳ��������ͼ��ĳߴ���ͬ�������ǵ�ͨ��
		result.create(image.size(), CV_8U);

		// ����ѭ��
		// ȡ�õ�����
		cv::Mat_<cv::Vec3b>::const_iterator it = image.begin<cv::Vec3b>();
		cv::Mat_<cv::Vec3b>::const_iterator itend = image.end<cv::Vec3b>();
		cv::Mat_<uchar>::iterator itout = result.begin<uchar>();

		// ����ÿ������
		for (; it != itend; ++it, ++itout) {
			// process each pixel ---------------------
			// �Ƚ���Ŀ����ɫ�Ĳ��
			if (getDistanceToTargetColor(*it)<maxDist) {
				*itout = 255;
			}
			else {
				*itout = 0;
			}
			// end of pixel processing ----------------
		}

		return result;
	}


	// ---------------------------------------------
	// Getters and setters
	// ������ɫ������ֵ����ֵ������������������Ϊ0
	void setColorDistanceThreshold(int distance) {
		if (distance < 0)distance = 0;
		maxDist = distance;
	}

	// ȡ����ɫ������ֵ
	int getColorDistanceThreshold() const {
		return maxDist;
	}

	// ����RGBɫ�ʿռ�����Ҫ������ɫ
	void setTargetColor(uchar blue, uchar green, uchar red) {
		// ����ΪRGB
		target = cv::Vec3b(blue, green, red);
	}

	// ������Ҫ������ɫ
	void setTargetColor(cv::Vec3b color) {
		target = color;
	}

	// ȡ����Ҫ������ɫ
	cv::Vec3b getTargetColor() const {
		return target;
	}
};