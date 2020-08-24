#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像读取与显示
int OpencvDemo001() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

//	cv::Mat src = cv::imread("images/test.png");
	cv::Mat src = cv::imread("images/test.png", cv::IMREAD_GRAYSCALE);
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	cv::imshow("input", src);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
