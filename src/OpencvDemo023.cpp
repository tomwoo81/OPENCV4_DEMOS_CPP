#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 中值模糊
int OpencvDemo023() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/sp_noise.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	cv::imshow("input", src);

	cv::Mat dst;
	cv::medianBlur(src, dst, 5);
	cv::imshow("median blur ksize=5", dst);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
