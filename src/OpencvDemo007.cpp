#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像像素的逻辑操作
int OpencvDemo007() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	// create image one
	cv::Mat src1 = cv::Mat::zeros(cv::Size(400, 400), CV_8UC3);
	cv::Rect rect(100, 100, 100, 100);
	src1(rect) = cv::Scalar(0, 0, 255);
	cv::imshow("input1", src1);
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, "create the first image.");

	// create image two
	cv::Mat src2 = cv::Mat::zeros(cv::Size(400, 400), CV_8UC3);
	rect.x = 150;
	rect.y = 150;
	src2(rect) = cv::Scalar(0, 0, 255);
	cv::imshow("input2", src2);
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, "create the second image.");

	// 逻辑操作
	cv::Mat dst1, dst2, dst3;
	cv::bitwise_and(src1, src2, dst1);
	cv::bitwise_xor(src1, src2, dst2);
	cv::bitwise_or(src1, src2, dst3);

	// show results
	cv::imshow("dst1", dst1);
	cv::imshow("dst2", dst2);
	cv::imshow("dst3", dst3);

	cv::Mat src = cv::imread("images/test.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	cv::imshow("input", src);

	// 取反操作
	cv::Mat dst;
	cv::bitwise_not(src, dst);
	cv::imshow("dst", dst);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
