#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class ColorDetector {
private:
	int maxDist;  // 允许的最小差距
	cv::Vec3b target;  // 目标颜色
	cv::Mat result;  // 存储二值映像结果的图像

public:
	// 空构造函数，在此初始化默认参数
	ColorDetector() : maxDist(100), target(0, 0, 0) {}

	// 计算与目标颜色的差距
	int getDistanceToTargetColor(const cv::Vec3b& color) const {
		return getColorDistance(color, target);
	}

	// 计算两个颜色之间的城区距离
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

	// 处理图像，返回单通道二值图像
	cv::Mat process(const cv::Mat &image)
	{

		// 必要时重新分配二值映像；与输入图像的尺寸相同，不过是单通道
		result.create(image.size(), CV_8U);

		// 处理循环
		// 取得迭代器
		cv::Mat_<cv::Vec3b>::const_iterator it = image.begin<cv::Vec3b>();
		cv::Mat_<cv::Vec3b>::const_iterator itend = image.end<cv::Vec3b>();
		cv::Mat_<uchar>::iterator itout = result.begin<uchar>();

		// 对于每个像素
		for (; it != itend; ++it, ++itout) {
			// process each pixel ---------------------
			// 比较与目标颜色的差距
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
	// 设置颜色差距的阈值，阈值必须是正数，否则设为0
	void setColorDistanceThreshold(int distance) {
		if (distance < 0)distance = 0;
		maxDist = distance;
	}

	// 取得颜色差距的阈值
	int getColorDistanceThreshold() const {
		return maxDist;
	}

	// 设置RGB色彩空间中需要检测的颜色
	void setTargetColor(uchar blue, uchar green, uchar red) {
		// 次序为RGB
		target = cv::Vec3b(blue, green, red);
	}

	// 设置需要检测的颜色
	void setTargetColor(cv::Vec3b color) {
		target = color;
	}

	// 取得需要检测的颜色
	cv::Vec3b getTargetColor() const {
		return target;
	}
};