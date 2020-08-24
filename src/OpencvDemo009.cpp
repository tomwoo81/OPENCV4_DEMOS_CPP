#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像色彩空间转换
int OpencvDemo009() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/cat.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	cv::imshow("input", src);

	// BGR to HSV
	cv::Mat hsv;
	cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);
	cv::imshow("hsv", hsv);

	// BGR to YUV
	cv::Mat yuv;
	cv::cvtColor(src, yuv, cv::COLOR_BGR2YUV);
	cv::imshow("yuv", yuv);

	// BGR to YCrCb
	cv::Mat ycrcb;
	cv::cvtColor(src, ycrcb, cv::COLOR_BGR2YCrCb);
	cv::imshow("ycrcb", ycrcb);

	cv::Mat src2 = cv::imread("images/greenback.png");
	if (src2.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::imshow("src2", src2);

	cv::cvtColor(src2, hsv, cv::COLOR_BGR2HSV);
	cv::Mat mask;
	cv::inRange(hsv, cv::Scalar(35, 43, 46), cv::Scalar(77, 255, 255), mask);
	cv::imshow("mask", mask);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
