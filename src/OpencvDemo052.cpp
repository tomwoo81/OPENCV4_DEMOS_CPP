#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 二值图像分析—用几何矩计算轮廓中心与横纵比过滤
int OpencvDemo052() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/stuff.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// 去噪声与二值化
	cv::Mat binary;
	cv::Canny(src, binary, 80, 160, 3, false);

	// 形态学操作-膨胀
	cv::Mat k = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));
	cv::dilate(binary, binary, k);

	// 轮廓发现
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::findContours(binary, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point());

	cv::Mat dst = src.clone();

	for (uint i = 0; i < contours.size(); i++) {
		// 最小外接矩形
		cv::RotatedRect rotRect = cv::minAreaRect(contours[i]);
		float w = rotRect.size.width;
		float h = rotRect.size.height;

		// 计算横纵比
		float ratio = cv::min(w, h) / cv::max(w, h);
		CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("index: %d, ratio of the short side to the long side: %.3f", i, ratio));

		// 计算轮廓中心
		cv::Moments moments = cv::moments(contours[i]);
		double m00 = moments.m00;
		double m10 = moments.m10;
		double m01 = moments.m01;
		double cx = m10 / m00;
		double cy = m01 / m00;

		// 绘制轮廓
		if (ratio > 0.9) {
			cv::drawContours(dst, contours, i, cv::Scalar(0, 0, 255), 2, cv::LINE_8);
		}
		else if (ratio < 0.5) {
			cv::drawContours(dst, contours, i, cv::Scalar(0, 255, 0), 2, cv::LINE_8);
		}

		// 绘制轮廓中心
		cv::circle(dst, cv::Point2f(cx, cy), 2, cv::Scalar(255, 0, 0), cv::FILLED, cv::LINE_8, 0);
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with results of moments calculation", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::imshow("moments", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
