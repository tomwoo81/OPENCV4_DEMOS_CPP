#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// OpenCV中的基本阈值操作
int OpencvDemo041() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/master.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	cv::imshow("input", src);

	cv::Mat gray, dst;
	// 转换为灰度图像
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	double t = 127;

	/* THRESH_BINARY = 0,
	 * THRESH_BINARY_INV = 1,
	 * THRESH_TRUNC = 2,
	 * THRESH_TOZERO = 3,
	 * THRESH_TOZERO_INV = 4
	 */
	for (int i = 0; i < 5; i++) {
		cv::threshold(gray, dst, t, 255, i);
		cv::imshow(cv::format("thresholding - type %d", i), dst);
	}

	double ret;

	ret = cv::threshold(gray, dst, t, 255, cv::THRESH_OTSU);
	CV_LOG_DEBUG(CV_LOGTAG_GLOBAL, cv::format("threshold - otsu: %.0f", ret));
	cv::imshow("thresholding - otsu", dst);

	ret = cv::threshold(gray, dst, t, 255, cv::THRESH_TRIANGLE);
	CV_LOG_DEBUG(CV_LOGTAG_GLOBAL, cv::format("threshold - triangle: %.0f", ret));
	cv::imshow("thresholding - triangle", dst);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
