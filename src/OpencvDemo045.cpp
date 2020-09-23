#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static void threshold_1(const cv::Mat& src, cv::Mat& dst);
static void threshold_2(const cv::Mat& src, cv::Mat& dst);
static void threshold_3(const cv::Mat& src, cv::Mat& dst);

// 图像二值化与去噪
int OpencvDemo045() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/coins.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	int h = src.rows;
	int w = src.cols;

	// 二值化
	cv::Mat dst1;
	threshold_1(src, dst1);

	cv::Mat result1 = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result1(cv::Range(0, h), cv::Range(0, w)));
	cv::cvtColor(dst1, dst1, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
	dst1.copyTo(result1(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result1, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::putText(result1, "binary image", cv::Point(w + 10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::imshow("binarize - otsu", result1);

	// 高斯模糊去噪声与二值化
	cv::Mat dst2;
	threshold_2(src, dst2);

	cv::Mat result2 = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result2(cv::Range(0, h), cv::Range(0, w)));
	cv::cvtColor(dst2, dst2, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
	dst2.copyTo(result2(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result2, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::putText(result2, "binary image", cv::Point(w + 10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::imshow("binarize - gaussian blur & otsu", result2);

	// 均值迁移模糊去噪声与二值化
	cv::Mat dst3;
	threshold_3(src, dst3);

	cv::Mat result3 = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result3(cv::Range(0, h), cv::Range(0, w)));
	cv::cvtColor(dst3, dst3, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
	dst3.copyTo(result3(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result3, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::putText(result3, "binary image", cv::Point(w + 10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::imshow("binarize - mean shift filtering & otsu", result3);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

static void threshold_1(const cv::Mat& src, cv::Mat& dst) {
	cv::Mat gray;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, dst, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
}

static void threshold_2(const cv::Mat& src, cv::Mat& dst) {
	cv::Mat blurred, gray;
	cv::GaussianBlur(src, blurred, cv::Size(3, 3), 0, 0);
	cv::cvtColor(blurred, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, dst, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
}

static void threshold_3(const cv::Mat& src, cv::Mat& dst) {
	cv::Mat blurred, gray;
	cv::pyrMeanShiftFiltering(src, blurred, 10, 100);
	cv::cvtColor(blurred, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, dst, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
}

/* end of file */
