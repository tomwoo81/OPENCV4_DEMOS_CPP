#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// USM锐化增强算法
int OpencvDemo035() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/master.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::Mat blurred, dst;
	cv::GaussianBlur(src, blurred, cv::Size(0, 0), 25); // sigma = 5, 15, 25
	cv::addWeighted(src, 1.5, blurred, -0.5, 0, dst);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "sharpen image", cv::Point(w + 10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::imshow("unsharpen mask", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
