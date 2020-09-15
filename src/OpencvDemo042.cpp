#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// OTSU二值寻找算法
int OpencvDemo042() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/lena.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// 自动阈值寻找与二值化
	cv::Mat gray, dst;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	double t = cv::threshold(gray, dst, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
	CV_LOG_DEBUG(CV_LOGTAG_GLOBAL, cv::format("threshold - otsu: %.0f", t));

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "binary image", cv::Point(w + 10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::imshow("binarize - otsu", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
