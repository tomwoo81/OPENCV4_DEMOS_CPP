#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 二值图像分析—点多边形测试
int OpencvDemo057() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/my_mask.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// 二值化
	cv::Mat gray, binary;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

	// 轮廓发现
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(binary, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point());

	cv::Mat dst = cv::Mat::zeros(src.size(), CV_32FC3);
	for (int row = 0; row < src.rows; row++) {
		for (int col = 0; col < src.cols; col++) {
			double dist = pointPolygonTest(contours[0], cv::Point(col, row), true);

			if (dist == 0) {
				dst.at<cv::Vec3f>(row, col) = cv::Vec3f(255, 255, 255);
			}
			if (dist > 0) {
				dst.at<cv::Vec3f>(row, col) = cv::Vec3f(255 - dist, 0, 0);
			}
			if (dist < 0) {
				dst.at<cv::Vec3f>(row, col) = cv::Vec3f(0, 0, 255 + dist);
			}
		}
	}

	cv::convertScaleAbs(dst, dst);
	dst.convertTo(dst, CV_8UC3);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w, h * 2), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(h, h * 2), cv::Range(0, w)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 255, 0), 1);
	cv::putText(result, "image with results of point-in-contour testing", cv::Point(10, h + 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 255, 0), 1);
	cv::imshow("point-in-contour testing", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
