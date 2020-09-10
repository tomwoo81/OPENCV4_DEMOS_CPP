#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// Canny边缘检测器
int OpencvDemo036() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/master.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::Mat dst;
	cv::Canny(src, dst, 100, 300, 3, false);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "edge image", cv::Point(w + 10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::imshow("canny", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
