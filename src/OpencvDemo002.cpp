#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像色彩空间转换
int OpencvDemo002() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/test.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	imshow("input", src);

	cv::Mat gray;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	cv::imwrite("output/gray.png", gray);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
