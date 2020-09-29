#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 二值图像分析—轮廓发现
int OpencvDemo048() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/coins.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// 去噪声与二值化
	cv::Mat blurred, gray, binary;
	cv::GaussianBlur(src, blurred, cv::Size(3, 3), 0, 0);
	cv::cvtColor(blurred, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

	// 轮廓发现与绘制
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::findContours(binary, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point());
	cv::Mat dst1 = src.clone();
	cv::drawContours(dst1, contours, -1, cv::Scalar(0, 0, 255), 2, cv::LINE_8); // draw all contours found

	cv::findContours(binary, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point());
	cv::Mat dst2 = src.clone();
	cv::drawContours(dst2, contours, -1, cv::Scalar(0, 0, 255), 2, cv::LINE_8); // draw all contours found

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w, h * 3), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst1.copyTo(result(cv::Range(h, h * 2), cv::Range(0, w)));
	dst2.copyTo(result(cv::Range(h * 2, h * 3), cv::Range(0, w)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "image with all contours", cv::Point(10, h + 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "image with external contours", cv::Point(10, h * 2 + 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::imshow("contours finding", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
