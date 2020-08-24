#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 像素归一化
int OpencvDemo011() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/test.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	cv::imshow("input", src);

	cv::Mat gray, gray_f;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

	// 转换为浮点数类型数组
	gray.convertTo(gray, CV_32F);

	// scale and shift by NORM_MINMAX
	cv::Mat dst = cv::Mat::zeros(gray.size(), CV_32FC1);
	cv::normalize(gray, dst, 1.0, 0, cv::NORM_MINMAX);
	cv::Mat result = dst * 255;
	result.convertTo(dst, CV_8UC1);
	cv::imshow("NORM_MINMAX", dst);

	// scale and shift by NORM_INF
	cv::normalize(gray, dst, 1.0, 0, cv::NORM_INF);
	result = dst * 255;
	result.convertTo(dst, CV_8UC1);
	cv::imshow("NORM_INF", dst);

	// scale and shift by NORM_L1
	cv::normalize(gray, dst, 1.0, 0, cv::NORM_L1);
	result = dst * 10000000;
	result.convertTo(dst, CV_8UC1);
	imshow("NORM_L1", dst);

	// scale and shift by NORM_L2
	cv::normalize(gray, dst, 1.0, 0, cv::NORM_L2);
	result = dst * 10000;
	result.convertTo(dst, CV_8UC1);
	imshow("NORM_L2", dst);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
