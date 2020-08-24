#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像ROI与ROI操作
int OpencvDemo016() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/flower.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	cv::imshow("input", src);

	int h = src.rows;
	int w = src.cols;

	// 获取ROI
	int cy = h / 2;
	int cx = w / 2;
	cv::Rect rect(cx - 100, cy - 100, 200, 200);
	cv::Mat roi = src(rect);
	cv::imshow("roi", roi);

	// copy ROI
	cv::Mat image = roi.clone();

	// modify ROI
	roi.setTo(cv::Scalar(255, 0, 0));

	// modify copy roi
	image.setTo(cv::Scalar(0, 0, 255));
	cv::imshow("result", src);
	cv::imshow("copy roi", image);

	// example with ROI - generate mask
	cv::Mat src2 = cv::imread("images/greenback.png");
	if (src2.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::imshow("src2", src2);
	cv::Mat hsv, mask;
	cv::cvtColor(src2, hsv, cv::COLOR_BGR2HSV);
	cv::inRange(hsv, cv::Scalar(35, 43, 46), cv::Scalar(77, 255, 255), mask);
	cv::imshow("mask", mask);

	// extract person ROI
	cv::Mat person;
	cv::bitwise_not(mask, mask);
	cv::bitwise_and(src2, src2, person, mask);
	cv::imshow("person", person);

	// generate background
	cv::Mat result = cv::Mat::zeros(src2.size(), src2.type());
	result.setTo(cv::Scalar(255, 0, 0));

	// combine background + person
	cv::Mat dst;
	cv::bitwise_not(mask, mask);
	cv::bitwise_or(person, result, dst, mask);
	cv::add(dst, person, dst);
	cv::imshow("dst", dst);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
